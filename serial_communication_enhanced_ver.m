delete(instrfind)
obj=serial('com5','baud',9600,'terminator','CR'); %create a new serial object

%%--------------variable initialize--------------
fs=420; %the sampling frequency we use
T=0.2; %total data collection time
A=zeros(1,fs*T); %the fs*T must be integer
t=1/fs:1/fs:T; %time axis
%h=animatedline;
%axis([0 0.5 0 5])
obj.InputBufferSize=10000; %larger buffer size won't overflow
fopen(obj);

L=length(t);
N=100*L;




%%-------------receive data and plot----------------
%for n=1:10;
while(1)
    for i=1:length(A);
        temp=fgetl(obj);%save the data
       %if length(temp)<4 && length(temp)>0; %condition check if data is valid 
            A(i)=str2num(temp)*5/2^10;
            %addpoints(h,t(i),A(i))
            drawnow
        %else;
            %break; % quit the loop if data is invalid
        %end
    end
    %subplot(2,5,n);
    subplot(2,1,1);
    plot(t,A);
    ylim([0 5]);% scaled verticle axis
    title('Scope');
    xlabel('time(s)');
    ylabel('voltage(V)');
    X=fftshift(abs(fft(A,N)))/L;
    f=fs*linspace(-1,1,N)./2;
    subplot(2,1,2);
    plot(f,X);
    title('Fourier analysis');
    xlabel('frequency(Hz)');
    ylabel('Magnitude');
end

fclose(obj)
delete(obj)%delete the serial object after complete data collection

%%-------------further data analysis----------------
% L=length(t);
% N=100*L;
% X=fftshift(abs(fft(A,N)))/L;
% f=fs*linspace(-1,1,N)./2;
% figure
% plot(f,X)