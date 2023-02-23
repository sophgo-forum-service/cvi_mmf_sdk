{
	"invalid and of negative number",
	.insns = {
	BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
	BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
	BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
	BPF_LD_MAP_FD(BPF_REG_1, 0),
	BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_map_lookup_elem),
	BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
	BPF_LDX_MEM(BPF_B, BPF_REG_1, BPF_REG_0, 0),
	BPF_ALU64_IMM(BPF_AND, BPF_REG_1, -4),
	BPF_ALU64_IMM(BPF_LSH, BPF_REG_1, 2),
	BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
	BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, offsetof(struct test_val, foo)),
	BPF_EXIT_INSN(),
	},
	.fixup_map_hash_48b = { 3 },
	.errstr = "R0 max value is outside of the allowed memory range",
	.result = REJECT,
	.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
},
{
	"invalid range check",
	.insns = {
	BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
	BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
	BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
	BPF_LD_MAP_FD(BPF_REG_1, 0),
	BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_map_lookup_elem),
	BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 12),
	BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_0, 0),
	BPF_MOV64_IMM(BPF_REG_9, 1),
	BPF_ALU32_IMM(BPF_MOD, BPF_REG_1, 2),
	BPF_ALU32_IMM(BPF_ADD, BPF_REG_1, 1),
	BPF_ALU32_REG(BPF_AND, BPF_REG_9, BPF_REG_1),
	BPF_ALU32_IMM(BPF_ADD, BPF_REG_9, 1),
	BPF_ALU32_IMM(BPF_RSH, BPF_REG_9, 1),
	BPF_MOV32_IMM(BPF_REG_3, 1),
	BPF_ALU32_REG(BPF_SUB, BPF_REG_3, BPF_REG_9),
	BPF_ALU32_IMM(BPF_MUL, BPF_REG_3, 0x10000000),
	BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_3),
	BPF_STX_MEM(BPF_W, BPF_REG_0, BPF_REG_3, 0),
	BPF_MOV64_REG(BPF_REG_0, 0),
	BPF_EXIT_INSN(),
	},
	.fixup_map_hash_48b = { 3 },
	.errstr = "R0 max value is outside of the allowed memory range",
	.result = REJECT,
	.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
},
{
	"check known subreg with unknown reg",
	.insns = {
	BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_get_prandom_u32),
	BPF_ALU64_IMM(BPF_LSH, BPF_REG_0, 32),
	BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 1),
	BPF_ALU64_IMM(BPF_AND, BPF_REG_0, 0xFFFF1234),
	/* Upper bits are unknown but AND above masks out 1 zero'ing lower bits */
	BPF_JMP32_IMM(BPF_JLT, BPF_REG_0, 1, 1),
	BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_1, 512),
	BPF_MOV64_IMM(BPF_REG_0, 0),
	BPF_EXIT_INSN(),
	},
	.result = ACCEPT,
	.retval = 0
},