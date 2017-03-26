delete(instrfind)
obj=serial('com5','baud',9600,'terminator','CR'); %create a new serial object

%%--------------variable initialize--------------
fs=420; %the sampling frequency we use
T=1; %total data collection time
A=zeros(1,fs*T); %the fs*T must be integer
t=1/fs:1/fs:T; %time axis
%h=animatedline;
%axis([0 0.5 0 5])
obj.InputBufferSize=5000; %larger buffer size won't overflow
fopen(obj);

%%-------------receive data and plot----------------
%for n=1:10;
    for i=1:length(A);
        temp=fgetl(obj);%save the data
        if length(temp)<4 && length(temp)>0; %condition check if data is valid 
            A(i)=str2num(temp)*5/1024;
            %addpoints(h,t(i),A(i))
            %drawnow
        else;
            break; % quit the loop if data is invalid
        end
    end
    %subplot(2,5,n);
    plot(t,A);
    ylim([0 5]);% scaled verticle axis
    title('scope');
    xlabel('time(s)');
    ylabel('voltage(V)');
%end

fclose(obj)
delete(obj)%delete the serial object after complete data collection

%%-------------further data analysis----------------
% L=length(t);
% N=100*L;
% X=fftshift(abs(fft(A,N)))/L;
% f=fs*linspace(-1,1,N)./2;
% figure
% plot(f,X)
% xlim([-500 500])