
trainerror = dlmread('plottrainerr.txt',';')
testerror = dlmread('plottesterr.txt',';')
features = dlmread('plotepoch.txt',';')

xlabel('features')
ylabel('error')
title('Error plot SVD','FontSize',12)
legend('training error','testing error')
plot(features,trainerror,'b',features,testerror,'r');