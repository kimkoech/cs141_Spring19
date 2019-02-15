`timescale 1ns / 1ps
`default_nettype none //helps catch typo-related bugs
//////////////////////////////////////////////////////////////////////////////////
// 
// CS 141 - Fall 2015
// Module Name:    mux_8to1 
// Author(s): 
// Description: 
//
//
//////////////////////////////////////////////////////////////////////////////////
module mux_8to1(E,F,G,H,I,J,K,L,S,Z);

	//port definitions - customize for different bit widths
	input wire [31:0] E,F,G,H,I,J,K,L; // 8 32bit input buses
	input wire [2:0] S; // 3bit signal select bus
	output wire [31:0] Z; // 1 32bit output bus
	
	
	//instantiate hardware
   wire mux_0_out , mux_1_out;
	mux_4to1 MUX0 (.A(E) , .B(F) , .C(G), .D(H) , .S(S[1:0]), .Z(mux_0_out)); // 4 to 1
	mux_4to1 MUX1 (.A(I) , .B(J) , .C(K), .D(L) , .S(S[1:0]), .Z(mux_1_out )); // 4 to 1
	mux_2to1 MUX2 (.X(mux_0_out) , .Y(mux_1_out) , .S(S[2]), .Z(Z)) ; // 2 to 1


endmodule
`default_nettype wire //some Xilinx IP requires that the default_nettype be set to wire