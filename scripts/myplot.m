% Assumes a data.txt file in the current directory
load data.txt

% Plot command
plot(data(:,1),data(:,2),'.');

% Actually draw the thing
drawnow();

% Make sure the program does not quit immediately
input ("Press key to quit");

