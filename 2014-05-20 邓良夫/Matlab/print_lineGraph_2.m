function print_ans1()
    tmp = load('x-2.txt');
    
    x=tmp(:,4);
    y=tmp(:,5);
    t = 0:0.05:0.95;
    plot(t,y,'k-+','linewidth',1,'markersize',6,'color','r');
    hold on;
    plot(t,x,'k-*','linewidth',1,'markersize',6);
    axis([0,1,0,1]);
    ylabel('Accuracy');
    xlabel('Confidence Threshold');
    
    legend 'Effective Prediction Rate' 'Prediction Accuracy'
  %  set(gca,'xtick',x);
end