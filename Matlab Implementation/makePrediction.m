
function rcap = makePrediction(r,userfeature, moviefeature, U ,I)
    
    max_features = 40;
    rcap =0
    
    for f = 1:max_features
        rcap = rcap + userfeature(f,U(r))*moviefeature(f,I(r)); 
    end
    
end