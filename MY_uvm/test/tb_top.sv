import uvm_pkg::*;
`include "uvm_macros.svh"
`include "timescale.sv"

module tb_top;

  initial 
    begin : testcase
      `uvm_info("Shahul",$sformatf("%s : Start of Test",`PREFIX),
                 UVM_LOW)
      `uvm_info("Shahul",$sformatf("\n%s\n%s \n%s",`HDR, `HDR_LINE, `HDR), 
                 UVM_MEDIUM)
      `uvm_info("Shahul",$sformatf("%s : End of Test",`PREFIX), UVM_MEDIUM  )
      
      `uvm_warning("Shahul_UVM_warning","Sample Warning")
      `uvm_error("Shahul_UVM_error","Sample Error")
    end : testcase

endmodule: tb_top