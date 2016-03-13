% Controller/plotting settings-----------------
P = 150;                                        % Values with a factor of 1000
I = 150000;                                     % --||--
D = 0;                                          % --||--
SETDISTANCE = 200;                              % Setpoint in mm from sensor
OFFSET = 500;                                   % Offset 0-999
ANTIWINDUP = 100000;                            % Maximum integral sum
FREQUENCY = 20;                                 % Frequency for sampling(Hz)
COM_PORT = 'COM6';
BAUDRATE = 115200;
A = importdata('regulator4.txt');               % Import conversion array
PLOT_TIME = 20;                                 % Window/plotting size in seconds
% Connect to Arduino---------------------------
a = serial(COM_PORT, 'BaudRate', BAUDRATE); 
set(a, 'Terminator', 10);
fopen(a);
% Initialize vectors and matrixes for plotting--
values = rand(FREQUENCY*PLOT_TIME,4);           % Matrix which will be plotted
tempValues = zeros(FREQUENCY,3);                % Matrix holding 1 sec of values
setpointvector = ones(FREQUENCY*PLOT_TIME,1).*SETDISTANCE;
values(:,4) = setpointvector;                   % Insert a static setpoint vector
x = 1:FREQUENCY*PLOT_TIME;                      % x-vector for time vector
plotGraph = plot(x,values);                     % Set up Plot
pause(1)                                        % Let Matlab render
flushoutput(a);                                 % flush serial output/input 
flushinput(a);                                  % buffer before attempting to send
for index = 1:100                               % Transfer conversion array
    fprintf(a, num2str(A(index)));
end
fprintf(a, num2str(P));                         % Transfer controller values
fprintf(a, num2str(I));                         % --||--
fprintf(a, num2str(D));                         % --||--
fprintf(a, num2str(SETDISTANCE));               % --||--
fprintf(a, num2str(OFFSET));                    % --||--
fprintf(a, num2str(ANTIWINDUP));                % --||--
fprintf(a, num2str(1000/FREQUENCY));            % --||--
while ishandle(plotGraph)                       % While plotting window is active
    for index=1 : FREQUENCY                     % Read and insert values to temp matrix
        tempValues(1,1) = str2double(fscanf(a,'%c'));   
        tempValues(1,2) = str2double(fscanf(a,'%c'));
        tempValues(1,3) = str2double(fscanf(a,'%c'));        
        tempValues = circshift(tempValues, 1);  % Shift values downwards    
    end
    values = circshift(values, FREQUENCY);      % Shift big matrix to fit temporary matrix
    values(1:FREQUENCY,1:3) = tempValues;       % Insert temp matrix to plot latest values
    plotGraph = plot(x,values);                 % Plot latest values
    legend('Distance', 'error', 'output', 'setpoint');
    pause(0.5)                                  % Pause to let Matlab render
end
fclose(a);
plot(x,values);
legend('Distance', 'error', 'output', 'setpoint');

