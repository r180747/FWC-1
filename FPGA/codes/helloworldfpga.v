module helloworldfpga(

    input  wire X,
    input  wire Y,
    input  wire Z,
    
    output wire F,
    );
    

assign F=((!(X))|(!(Y))|(!(Z)))&((!(X))|Y|(!(Z)))&(X|(!(Y))|Z)&(X|Y|(!(Z))); 
endmodule
