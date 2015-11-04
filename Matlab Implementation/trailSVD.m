%trailSVD
%U=[1;1;1;2;2;3;3;3;4;4;5;5;5];
%I=[1;2;4;1;4;1;2;4;1;4;2;3;4];
%R=[5;3;1;4;1;1;1;5;1;4;1;5;4];

U = userMatrix(:,:);
I = itemMatrix(:,:);
R = ratingMatrix(:,:);
max_epochs = 120;
max_features = 40;
max_users = 943;
max_movies = 1682;
lrate = 0.001

userfeature(1:max_features,1:max_users)=0.1;
moviefeature(1:max_features,1:max_movies)=0.1;

%making rating prediction for user i and movie j 
uservalue = []
movievalue = []
ploterr = []
plotepoch = []

%training the model

for f = 1:max_features
    userValue = userfeature(f,:);
    movieValue = moviefeature(f,:);
    
    %finding error
    for epoch = 1:max_epochs
        for r = 1:size(R,1)
            err = R(r,1) - makePrediction(r,userfeature(:,:),moviefeature(:,:),U,I);
            uv = userValue(1,U(r,1))
            userValue(1,U(r,1))= userValue(1,U(r,1))+ lrate*err*movieValue(1,I(r,1)); 
            movieValue(1,I(r,1))= movieValue(1,I(r,1))+ lrate*err*uv;
            userfeature(f,U(r))= userValue(1,U(r,1))
            moviefeature(f,I(r))= movieValue(1,I(r,1))
        end
        ploterr = [ploterr;err];
        plotepoch = [plotepoch;epoch]
    end
end

size(ploterr)
size(plotepoch)
plot(plotepoch,ploterr)





