function print_ans1(x,y,t)
    tmp = load('x-1.txt');
    t=0:0.01:0.19;
    x=tmp(:,4);
    y=tmp(:,5);
    plot(t,y,'k-+','linewidth',1,'markersize',6,'color','r');
    hold on;
    plot(t,x,'k-*','linewidth',1,'markersize',6);
    axis([0,0.2,0,1]);
    ylabel('Accuracy');
    xlabel('Minsup');
    
     legend 'Effective Prediction Rate' 'Prediction Accuracy'
  %  set(gca,'xtick',x);
end