#include <iostream>
#include <fstream>
#include <memory.h>
#include <cassert>
using namespace std;


// initializing the variables for training the SVD model
    int max_epochs = 120;
    const int max_features = 60;
    const int max_users = 943;
    const int max_movies = 1682;
    double lrate = 0.0001;

double makeprediction ( int user, int movie, double uf[max_features][max_users],double mf[max_features][max_movies], int cf, double avg[max_movies+1], double userOffset[max_users+1])
    {
        double retval = avg[movie] + userOffset[user];

        for(int f =0; f <= cf; f++)
        {
            retval = retval + (uf[f][user]*mf[f][movie]);
        }
        return retval;
    }


    int U[80000];
    int I[80000];
    int R[80000];
    int Utest[20000];
    int Itest[20000];
    int Rtest[20000];

int main()
{
    //declaring variables for assigning values to input arrays.
    int n,i,j;

    //assigning values to the input arrays of the training model.
    i=0;
    ifstream fu("userMatrixtrain.txt");

    while( fu >> n)
        {
            U[i] = n;
            i++;
        }
    assert( i <= 80000);

    i=0;
    ifstream fi("itemMatrixtrain.txt");
    while( fi >> n)
        {
	  I[i] = n;
            i++;
        }
    assert(i <= 80000);
    i=0;
    ifstream fr("ratingMatrixtrain.txt");

    while( fr >> n)
        {
            R[i] = n;
            i++;
        }
    assert(i <= 80000);


    //assigning values to the input arrays of the testing model.
    i=0;
    ifstream futest("userMatrixtest.txt");

    while( futest >> n)
        {
            Utest[i] = n;
            i++;
        }
    assert( i <= 20000);

    i=0;
    ifstream fitest("itemMatrixtest.txt");

    while( fitest >> n)
        {
            Itest[i] = n;
            i++;
        }
    assert( i <= 20000);

    i=0;
    ifstream frtest("ratingMatrixtest.txt");

    while( frtest >> n)
        {
            Rtest[i] = n;
            i++;
        }
    assert( i <= 20000);

    //declaring the user feature and movie feature array
    double userfeature[max_features][max_users];
    double moviefeature[max_features][max_movies];

    //initializing the userfeature array with 0.1
    i=0; j=0;

    for (i= 0 ; i<max_features ; i ++)
        {
            for (j= 0 ; j<max_users ; j ++)
                {
                        userfeature[i][j]=0.1;
                        //cout << "i:\t" << i << "\t j:" << j << "\t value:" << userfeature[i][j] << "\n";
                }
        }

    //initializing the moviefeature array with 0.1
    i=0; j=0;

    for (i= 0 ; i<max_features ; i ++)
        {
            for (j= 0 ; j<max_movies ; j ++)
                {
                        moviefeature[i][j]=0.1;
                        //cout << "i:\t" << i << "\t j:" << j << "\t value:" << moviefeature[i][j] << "\n";
                }
        }

    //********************************************************************************************************************************************

    // Finding the average rating of all movies
    int max_ratings = 80000;
    float sum = 0,avg=0;

    for (int i = 0; i < max_ratings; i++)
    {
       sum += R[i];
    }
    avg = sum/80000;
    cout << "average:" << avg << endl;


    //Finding out the average of each movie

    float count =0;
    double average[max_movies+1];
    int movies = 1;
    float x= 0;
    average[0]=0;
    j=1;
    sum = 0;

    while (movies <= max_movies)
        {
            for (int i =0; i < max_ratings; i++)
            {
            	if (I[i] == movies)
                {
                sum = sum + R[i];
                count = count + 1;
                }
            }
		x = sum/count;
		if (count == 0)
            {
		    cout << "BAD MOVIE: " << movies << endl;
            average[j] = avg;
            }
        else
            {
            cout << "Good movie:" << movies << endl;
            average[j] = x;
            }
        sum = 0;
        count = 0;
        x=0;
        movies++;
        j++;
        }

    /*for (int i = 0; i <= max_movies; i++)
    {
        cout << "Movie:" << i << "\taverage:" << average[i] << endl;
        y = average[i];
    }*/
    //cout << "Movie:1604" <<"\taverage:" << average[1604] << endl;
    //********************************************************************************************************************************************
    //Finding out user offset for each movie

    int users = 1, offset_sum = 0;
    double user_offset[max_users+1];
    user_offset[0]= 0;

    while (users <= max_users)
        {
            count = 0;
            offset_sum=0;
            for (int i=0; i < max_ratings ;i++)
            {
                if (U[i] == users)
                {
                    offset_sum += R[i] - average[I[i]];
                    count += 1;
                }
            }
           x = offset_sum/count;
           //if (count == 0) cout << "BAD USER: " << users << endl;

           user_offset[users] = x;
           users++;
        }


    //********************************************************************************************************************************************

    //training and testing the SVD model

    //#######################################################################################
    int f, epoch, current_feature;
    double trainerr,testerr,msetrainerr,msetesterr;
    double *uservalue;
    double *movievalue;
    double val,uv;
    //#######################################################################################
    int max_ratingstest;
    max_ratingstest = 20000;
    cout << "\t max test ratings:" << max_ratingstest;

    // storing values of training and testing data in files
    ofstream ftrain;
    ftrain.open ("plottrainerr.txt");

    ofstream ftest;
    ftest.open ("plottesterr.txt");

    ofstream ffeature;
    ffeature.open ("plotepoch.txt");


    for (f = 0; f < max_features; f++)
    {
        cout << "\t Train feature:" << f;
        uservalue = userfeature[f];
        movievalue = moviefeature[f];
        current_feature = f;

        for (epoch =0; epoch < max_epochs; epoch++)
        {

            // running the model on train data
            for (int r =0; r < max_ratings; r++)
            {
                val = makeprediction(U[r], I[r], userfeature, moviefeature, current_feature, average, user_offset);
                trainerr = R[r] - val;
                uv = uservalue[U[r]];
                uservalue[U[r]] += lrate*trainerr*movievalue[I[r]];
                movievalue[I[r]] += lrate*trainerr*uv;
            }

            msetrainerr = 0;

            // finding the error on training data
            for (int r =0; r < max_ratings; r++)
            {
                val = makeprediction(U[r], I[r], userfeature, moviefeature, current_feature, average, user_offset);
                trainerr = R[r] - val;
                trainerr = trainerr * trainerr;
                msetrainerr += trainerr;

            }

            msetrainerr = msetrainerr/max_ratings;

            // running the model on test data

            msetesterr = 0;

            for (int r =0; r <max_ratingstest; r++)
                {
                    val = makeprediction(Utest[r], Itest[r], userfeature, moviefeature, current_feature, average, user_offset);
                    //cout << "Rating " << Rtest[r] << " pred " << val << endl;
                    testerr = Rtest[r] - val;
                    testerr = testerr * testerr;
                    msetesterr += testerr;
                }
	        //cout << "test count: " << max_ratingstest << endl;
            msetesterr = msetesterr/max_ratingstest;

            if (epoch == max_epochs - 1)
            {
                ftrain << msetrainerr << "\n";
                ftest << msetesterr << "\n";
                ffeature << f << "\n";
            }
        }
    }

    ftest.close();
    ftrain.close();
    ffeature.close();

}










