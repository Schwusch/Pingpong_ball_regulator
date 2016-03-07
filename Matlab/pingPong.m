% Controller/plotting settings-----------------
P = 150;                                        % Values with a factor of 1000
I = 150000;                                     % --||--
D = 0;                                          % --||--
SETDISTANCE = 200;                              % Setpoint in mm from sensor
TIMER = 50;                                     % Sampling time in ms
COM_PORT = 'COM6';
BAUDRATE = 115200;
A = importdata('regulator4.txt');               %Import conversion array
plot_time = 20;                                 %Window/plotting size in seconds
%-----------------------------------------------
a = serial(COM_PORT, 'BaudRate', BAUDRATE); 
set(a, 'Terminator', 10);
fopen(a);
disp('Connected to Arduino')
freq = 1000/TIMER;
values = rand(freq*plot_time,4);
tempValues = zeros(freq,3);
setpointvector = ones(freq*plot_time,1).*SETDISTANCE;
values(:,4) = setpointvector;
x = 1:freq*plot_time;
plotGraph = plot(x,values);                     %Set up Plot
pause(1)
flushoutput(a);                                 %flush serial output/input 
flushinput(a);                                  %buffer before attempting to send
index = 1;
while (index < 101)                             % Transfer conversion array
    str = num2str(A(index));
    fprintf(a, str);
    index = index + 1;
end
fprintf(a, num2str(P));                         % Transfer controller values
fprintf(a, num2str(I));                         % --||--
fprintf(a, num2str(D));                         % --||--
fprintf(a, num2str(SETDISTANCE));               % --||--
fprintf(a, num2str(TIMER));                     % --||--
while ishandle(plotGraph)                       % While plotting window is active
    index = 1;  
    while(index < freq + 1)                     % Read and insert values to temp matrix
        tempValues(1,1) = str2double(fscanf(a,'%c'));   
        tempValues(1,2) = str2double(fscanf(a,'%c'));
        tempValues(1,3) = str2double(fscanf(a,'%c'));        
        tempValues = circshift(tempValues, 1);  % Shift values downwards
        index = index + 1;        
    end
    values = circshift(values, freq);           % Shift big matrix to fit temporary matrix
    values(1:freq,1:3) = tempValues;            % Insert temp matrix to plot latest values
    plotGraph = plot(x,values);                 % Plot latest values
    legend('Distance', 'error', 'output', 'setpoint');
    pause(0.5)                                  % Pause to let Matlab render
end
fclose(a);
plot(x,values);
legend('Distance', 'error', 'output', 'setpoint');

