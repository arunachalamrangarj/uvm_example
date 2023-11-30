
UVM_HOME ?= ../uvm_src/uvm-1.2/

run: clean uvm_lib 
	vlog -sv +define+UVM_NO_DPI +incdir+$(UVM_HOME)/src ../test/tb_top.sv -l sample_comp.log
	vsim -l sample_run.log -c +UVM_VERBOSITY=UVM_HIGH tb_top -do "run -all;exit"
	 
uvm_lib:
	vlib work
	vlog -sv +define+UVM_NO_DPI +incdir+$(UVM_HOME)/src $(UVM_HOME)/src/uvm_pkg.sv
	rm -fr uvm_dpi

clean:
	rm -fr work*  *.log