%% Serial Import - test code
% Andre Henry
% Date: 4/20/24

%
% Code to test the importaiton of data from Arduino microcontroller over
% serial comunicatoin. Code is also to test the saveing of data and live
% GUI of data.
%

%delete(instrfind) % Close existing serial port obj
% instrfind find serial port objects in use

clc
clear 

%% Serial Setup
s1 = serialport('COM3', 115200) % Define COM used at Buadrate
configureTerminator(s1, "CR") % Configure terminator to end w/ carrage return
% Note: Serial.print terminates the line with BOTHcarage return and linefeed
% charaters \r\n

%% Reading data
run = true;
n = 1;
while run
    data = readline(s1)
    n = n+1;
    % Assuming 'data' is a line read from the serial port
    fields = strsplit(data, ',');  % Split the string at semicolons

    % Convert and store each field
    intdata1 = str2double(fields{1});
    intdata2 = str2double(fields{2});
    floatdata3 = str2double(fields{3});
    %stringPart = fields{3};

    % Use or display the parsed data
    fprintf('Integer: %d, Float: %f, String: %f\n', intdata1, intdata2, floatdata3);

end

