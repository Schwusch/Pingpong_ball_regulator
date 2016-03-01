%program allready connected to arduino, due to 'a' as argument
%Before calling this function, one must use command:
% a  = arduino_com('COMPORT')

%s = serial('com3', 'BaudRate', 115200); fopen(s); fprintf(s, 'azazazaz'); fclose(s);

a = serial('COM14', 'BaudRate', 115200); 
set(a, 'Terminator', 10);
fopen(a);

disp('Connected to Arduino')

LISTENING = 0;
SENDING = 1;
flushinput(a);
A = importdata('regulator1.txt');
index = 1;
P = 1000;
I = 0;
D = 0;
SETDISTANCE = 300;
TIMER = 50;
freq = 1000/TIMER;
plot_time = 20;
values = rand(freq*plot_time,3);
tempValues = zeros(freq,3);
x = 1:freq*plot_time;

%User Defined Properties 
plotTitle = 'Ping & Pong';  % plot title
xLabel = 'Elapsed Time (s)';    % x-axis label
yLabel = 'Data';                % y-axis label

%Set up Plot
plotGraph = plot(x,values);
pause(1)
%linkdata on
title(plotTitle,'FontSize',25);
xlabel(xLabel,'FontSize',15);
ylabel(yLabel,'FontSize',15);



while(SENDING)
    % create String to send
    %flush serial output buffer before attempting to send
    flushoutput(a);
    while (index < 101)
        str = num2str(A(index));
        %disp(['Sending string: ' str ])
        fprintf(a, str);
        index = index + 1;
        flushoutput(a);
    end
    
    %disp(A)
    SENDING = 0;
end

str = num2str(P);
fprintf(a, str);
flushoutput(a);

str = num2str(I);
fprintf(a, str);
flushoutput(a);

str = num2str(D);
fprintf(a, str);
flushoutput(a);

str = num2str(SETDISTANCE);
fprintf(a, str);
flushoutput(a);

str = num2str(TIMER);
fprintf(a, str);
flushoutput(a);

while ishandle(plotGraph)
    
    index = 1;  
    %read and insert values to temp matrix
    while(index < freq + 1)
    
        %disp('collecting AVSTÅND data')
        tempValues(1,1) = str2double(fscanf(a,'%c'));   
        %disp('DONE collecting AVSTÅND data')
        %pause(1)
        
        tempValues(1,2) = str2double(fscanf(a,'%c'));
        %disp('DONE collecting REGLERFEL data')
         %pause(1)
        
        tempValues(1,3) = str2double(fscanf(a,'%c'));
        %disp('DONE collecting STYRVÄRDE data')
        %pause(1)
        
        tempValues = circshift(tempValues, 1);
        
        index = index + 1;
        
    end
    disp(tempValues)
    values = circshift(values, freq);
    
    values(1:freq,1:3) = tempValues;
    
    plotGraph = plot(x,values);
    pause(0.1)

end

fclose(a);
