%% Serial Import - Test code
% Andre Henry
% Date: 4/22/24

%
% Code to test the importation of data from Arduino Microcontroller over
% serial communication. Code is also to test the saving of data and live
% GUI of data.
%

clc
clear 

%% Serial Setup
s1 = serialport('COM3', 115200) % Define COM used at Buad rate
configureTerminator(s1, "CR") % Configure terminator to end w/ carriage return
% Note: Serial.print terminates the line with BOTH carriage return and linefeed
% characters \r\n

%% Reading data
% initialized data storage variables
run = true;
n = 1;

runTime = 60;
sampleTime = 1;

while run == true
    % Read data by line, ends with terminator from serial
    data = readline(s1);
    
    % Split the string at comas
    fields = strsplit(data, ',');

    % Convert and store each field
    t(n) = str2double(fields{1});
    a(n) = str2double(fields{2});
    v(n) = str2double(fields{3});
    z(n) = str2double(fields{4});
    ang(n) = str2double(fields{5});
    r(n) = str2double(fields{6});

    fprintf('Current time in sim: %.2f\n', t(n))

    % Generic code for data
    %intPart = str2double(fields{n});
    %floatPart = str2double(fields{n});
    %stringPart = fields{n};

    n = n+1;
    %check for complete sim
    if n > (runTime/sampleTime)
        run = false
        clear s1
    end

end

%% Plot Data
plot3(r.*sin(ang),r.*cos(ang),z)
title('Rocket Trajectory')
xlabel('X position')
ylabel('Y position')
zlabel('Z Altitude')
