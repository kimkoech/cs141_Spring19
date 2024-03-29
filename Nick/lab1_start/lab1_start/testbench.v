`timescale 1ns / 1ps
`default_nettype none

//the following module is made for SIMULATION ONLY - most of the language
//constructs used here will not synthesize, but will simulate
module testbench();
	reg [7:0] switch; // input
	wire [7:0] led; // output

	// instantiate unit under test (uut)
	main uut (switch, led);

	integer error = 0;

	initial begin
	
		for (switch = 0; switch != 8'b11111111; switch = switch + 1) begin
			#10; // wait for inputs to propagate

			// Test for part 1
			if (switch[0] && switch[1] !== led[0]) begin
				$display("ERROR: switch[0]=%b, switch[1]=%b, led[0]=%b", switch[0], switch[1], led[0]);
				error = error + 1;
			end

			// Test for part 2
			if (~(switch[2] && ~switch[3]) !== led[1]) begin
				$display("ERROR: switch[2]=%b, switch[3]=%b, led[1]=%b", switch[2], switch[3], led[1]);
				error = error + 1;
			end
			
			// Test for part 3   not great design but brute force works too
			if ((switch[0]+switch[1]+switch[2]+switch[3]+switch[4]+switch[5]+switch[6]+switch[7]) % 2 == led[2]) begin 
				$display("ERROR: switch=%b, led[2]=%b", switch, led[1]);
				error = error + 1;
			end
			
			// Test for part 4
			if ((switch[4]==0 && led[3]!==led[1]) || (switch[4]==1 && led[3]!==led[0])) begin
				$display("ERROR: switch=[4]=%b, led[3]=%b, led[1]=%b, led[0]=%b", switch[4], led[3], led[1], led[0]);
				error = error + 1;
			end
			
		end
		#10
		$display("Finished with %d errors", error);
		$finish;
	end
endmodule

`default_nettype wire
