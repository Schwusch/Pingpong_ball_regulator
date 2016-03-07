% Controller/plotting settings-----------------
P = 150;
I = 150000;
D = 0;
SETDISTANCE = 200;
TIMER = 50;
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
while (index < 101)
    str = num2str(A(index));
    fprintf(a, str);
    index = index + 1;
end
fprintf(a, num2str(P));
fprintf(a, num2str(I));
fprintf(a, num2str(D));
fprintf(a, num2str(SETDISTANCE));
fprintf(a, num2str(TIMER));
while ishandle(plotGraph)    
    index = 1;  
    while(index < freq + 1)                     %read and insert values to temp matrix
        tempValues(1,1) = str2double(fscanf(a,'%c'));   
        tempValues(1,2) = str2double(fscanf(a,'%c'));
        tempValues(1,3) = str2double(fscanf(a,'%c'));        
        tempValues = circshift(tempValues, 1);        
        index = index + 1;        
    end
    values = circshift(values, freq);    
    values(1:freq,1:3) = tempValues;
    
    plotGraph = plot(x,values);
    legend('Distance', 'error', 'output', 'setpoint');
    pause(0.5)
end
fclose(a);
plot(x,values);
legend('Distance', 'error', 'output', 'setpoint');

