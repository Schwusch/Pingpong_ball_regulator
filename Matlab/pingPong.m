
% Controller values
%----------------------------------------------
P = 150;
I = 150000;
D = 0;
SETDISTANCE = 200;
TIMER = 50;
COM_PORT = 'COM6';
BAUDRATE = 115200;
%Import conversion array
A = importdata('regulator1.txt');
%-----------------------------------------------
a = serial(COM_PORT, 'BaudRate', BAUDRATE); 
set(a, 'Terminator', 10);
fopen(a);
disp('Connected to Arduino')
SENDING = 1;
flushinput(a);
index = 1;
freq = 1000/TIMER;
plot_time = 20;
values = rand(freq*plot_time,4);
tempValues = zeros(freq,3);
setpointvector = ones(freq*plot_time,1).*SETDISTANCE;
values(:,4) = setpointvector;
x = 1:freq*plot_time;
%Set up Plot
plotGraph = plot(x,values);
pause(1)
%flush serial output buffer before attempting to send
flushoutput(a);
while (index < 101)
    str = num2str(A(index));
    fprintf(a, str);
    index = index + 1;
    flushoutput(a);
end
fprintf(a, num2str(P));
fprintf(a, num2str(I));
fprintf(a, num2str(D));
fprintf(a, num2str(SETDISTANCE));
fprintf(a, num2str(TIMER));

while ishandle(plotGraph)    
    index = 1;  
    %read and insert values to temp matrix
    while(index < freq + 1)
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

