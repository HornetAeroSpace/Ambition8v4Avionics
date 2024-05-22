%% Serial Import - Test code
% Andre Henry
% Date: 5/10/24

%
% Code to import the data from Arduino Microcontroller over
% serial communication, live data viewing and 
%

clc
clear 

%% Serial Setup
serial1 = serialport('COM3', 115200) % Define COM used at Buad ratebin2dec
configureTerminator(serial1, "CR/LF") % Configure terminator
% Note: Serial.print terminates the line with BOTH carriage return and linefeed
% characters \r\n

%% Data Preallocation

% Preallocate arrays for speed
CVel = NaN(1000,1);
CAcc = NaN(1000,1);
Calt = NaN(1000,1);
SVel = NaN(1000,1);
SAcc = NaN(1000,1);
SAlt = NaN(1000,1);
SOrient = NaN(1000,1);
SPortStatus = NaN(1000,1);
SARMStatus = NaN(1000,1);

%% Reading data
% initialized data storage variables
run = true;
n = 2;

% initialize plot data
% plot3((CAcc.*sin(ang)),(CAcc.*cos(ang)),SAlt)
% plot3(CAcc(n-1:n).*sin(ang(n-1:n)),CAcc(n-1:n).*cos(ang(n-1:n)),SAlt(n-1:n),"Color",'b')
%plot3(r(n).*sin(pi/4),r(n).*cos(pi/4),z(n))
title('Rocket Trajectory')
hold on, grid on
xlabel('X position')
ylabel('Y position')
zlabel('Z Altitude')


while run == true % Loop for data acquisition during test

    % Read data by line with error capture
    try
        data = readline(serial1); % Serial read

        dataFields = strsplit(data, ','); % conversion from string to array

        % Conversation and storage of data by device
        switch dataFields(1) 
            case "DataCOTS1:"
                tic;
                CVel(n) = str2double(dataFields{2});
                CAcc(n) = str2double(dataFields{3});
                Calt(n) = str2double(dataFields{4});
            case "DataSARD1:"
                SVel(n) = str2double(dataFields{2});
                SAcc(n) = str2double(dataFields{3});
                SAlt(n) = str2double(dataFields{4});
            case "DataSARD2:"
                SOrient(n) = str2double(dataFields{2});
                SPortStatus(n) = boolean(dataFields{3});
                SARMStatus(n) = boolean(dataFields{4});
            case "end"
                tic;
                % %plot3((r.*sin(ang)),(r.*cos(ang)),z)
                %plot3(CAcc(n-1:n).*sin(ang(n-1:n)),CAcc(n-1:n).*cos(ang(n-1:n)),SAlt(n-1:n),"Color",'b')
                title('Rocket Trajectory')
                hold on, grid on
                xlabel('X position')
                ylabel('Y position')
                zlabel('Z Altitude')
                n = n+1;
                toc;
            case "End of Sim"
                % End data acquisition upon completion message
                run = false;
                
            otherwise
                % Communication error catches
                if (dataFields(1) == " ")
                    break;
                end
                warning("Error in data collection.")
        end
    catch error
        disp(error.message)
    end
end

flightdata = table(CVel, CAcc, Calt, SVel, SAcc, SAlt, SOrient, SPortStatus, SARMStatus)

% Saving the data in .mat format
save('flightData.mat');
% Saving the data in .csv format
writetable(flightdata, 'flightdata.csv');


%% Plot Data
% plot3(r.*sin(ang),r.*cos(ang),z)
% title('Rocket Trajectory')
% xlabel('X position')
% ylabel('Y position')
% zlabel('Z Altitude')
