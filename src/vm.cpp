#include "word.h"

typedef void(*SeInstruction)();

class Vm {
private:
  // Registers
  struct se_word reg_A;
  struct se_word reg_S;
  struct se_word reg_T;
  struct se_word reg_Q;
  struct se_word reg_FP;
  struct se_word reg_SH;
  struct se_word reg_SB;
  short int sreg_IR;
  short int sreg_FL;
  // Memory
  struct se_word physmem[4096 * 512];

  // Instruction Lookup Table
  SeInstruction instruction_table[100] = {this->se_mr,
					  this->se_mm,
					  this->se_ad,
					  this->se_sb
					  this->se+dv,
					  this->se_bru,
					  this->se_brc,
					  this->se_lt,
					  this->se_gt,
					  this->se_ne,
					  this->se_ng,
					  this->se_and,
					  this->se_or,
					  this->se_xor,
					  this->se_not,
					  this->se_call,
					  this->se_chtp,
					  this->se_rad,
					  this->se_rsb,
					  this->se_rml,
					  this->se_rdv,
					  this->se_msr,
					  this->se_dam,
					  this->se_iam,
					  this->se_cktp};

  // Instructions
  static void se_mr();
  static void se_mm();
  static void se_ad();
  static void se_sb();
  static void se_dv();
  static void se_bru();
  static void se_brc();
  static void se_lt();
  static void se_gt();
  static void se_ne();
  static void se_eq();
  static void se_ng();
  static void se_and();
  static void se_or();
  static void se_xor();
  static void se_not();
  static void se_call();
  static void se_chtp();
  static void se_rad();
  static void se_rsb();
  static void se_rml();
  static void se_rdv();
  static void se_msr();
  static void se_dam();
  static void se_iam();
  static void se_cktp();

public:
  void step_cycle();
  void load_data();

};
