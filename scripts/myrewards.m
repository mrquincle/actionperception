% Assumes a data.txt file in the current directory
load rewards.txt

% Plot command
plot(rewards(:,1),rewards(:,2),'.');

% Actually draw the thing
drawnow();

% Make sure the program does not quit immediately
input ("Press key to quit");

