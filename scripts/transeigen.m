A = [0.49 0.21 0.21 0.09;
 0.7 0 0.3 0;
 0.7 0.3 0 0;
 1 0 0 0];

A = [0 0 1/2 0 1/2;
 0 0 1 0 0;
 1/4 1/4 0 1/4 1/4;
 0 0 1/2 0 1/2;
 0 0 0 0 1]

[w,la]=eig(A);

la=diag(la);
imax=find (la==max(real(la)));
v=conj(inv(w));
w1=abs(w(:,imax)); 
v1=abs(real (v(imax,:)))';

w1
v1

%la=diag(la)
%v=conj(inv(w))

%w
%la
