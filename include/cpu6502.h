/* Include guard */
#include <wchar.h>
#if !defined(CPU6502_H)
#define CPU6502_H

/* Includes */
#include <stdint.h>
#include <stddef.h>

/* Constants */
/* Size of RAM, in bytes */
#define RAM_SIZE            (2*1024*1024)
/* Size of a page, in bytes */
#define PAGE_SIZE           256
/* NMI Interrupt vector */
#define NMI_VECTOR          0xfffa
/* IRQ Interrupt vector */
#define IRQ_VECTOR          0xfffe
/* RESET Interrupt vector */
#define RESET_VECTOR        0xfffc

/* Addressing modes */
enum addressing_modes_6502 {
  ADDR_MODE_ACCUMULATOR=0,  /* The value in a */
  ADDR_MODE_ABSOLUTE=1,     /* The value at immediate address */
  ADDR_MODE_ABSOLUTE_X=3,   /* the value at immediate address + x */
  ADDR_MODE_ABSOLUTE_Y=4,   /* the value at immediate address + y */
  ADDR_MODE_IMMEDIATE=5,    /* The immediate value */
  ADDR_MODE_IMPLIED=6,      /* No value needed or value implied by instruction */
  ADDR_MODE_INDIRECT=7,     /* The value at address at immediate address */
  ADDR_MODE_INDIRECT_X=8,   /* The value at address at (immediate address + x) */
  ADDR_MODE_INDIRECT_Y=9,   /* (The value at address at immediate address) + y */
  ADDR_MODE_RELATIVE=10,     /* The value is the program counter + immediate */
  ADDR_MODE_ZERO_PAGE=11,    /* The value at immediate address in zero page */
  ADDR_MODE_ZERO_PAGE_X=12,  /* The value at immediate address in zero page + x */
  ADDR_MODE_ZERO_PAGE_Y=13,  /* The value at immediate address in zero page + y */
  ADDR_MODE_NONE=14,         /* Empty space in instruction set */
};
/* Instruction types */
enum instr_types_6502 {
  /* Load/store */
  INSTR_TYPE_LDA=0,         /* Load accumulator */
  INSTR_TYPE_LDX=1,         /* Load X register */
  INSTR_TYPE_LDY=2,         /* Load Y register */
  INSTR_TYPE_STA=3,         /* Store accumulator */
  INSTR_TYPE_STX=4,         /* Store X register */
  INSTR_TYPE_STY=5,         /* Store Y register */
  /* Register transfers */
  INSTR_TYPE_TAX=6,         /* Transfer accumulator to X */
  INSTR_TYPE_TAY=7,         /* Transfer accumulator to Y */
  INSTR_TYPE_TXA=8,         /* Transfer X to accumulator */
  INSTR_TYPE_TYA=9,         /* Transfer Y to accumulator */
  /* Stack operations */
  INSTR_TYPE_TSX=10,        /* Transfer stack pointer to X */
  INSTR_TYPE_TXS=12,        /* Transfer X to stack pointer */
  INSTR_TYPE_PHA=13,        /* Push accumulator */
  INSTR_TYPE_PHP=14,        /* Push status register */
  INSTR_TYPE_PLA=15,        /* Pop accumulator */
  INSTR_TYPE_PLP=16,        /* Pop status register */
  /* Logical */
  INSTR_TYPE_AND=17,        /* Logical AND */
  INSTR_TYPE_EOR=18,        /* Exclusive OR */
  INSTR_TYPE_ORA=19,        /* Logical OR */
  INSTR_TYPE_BIT=20,        /* Bit test */
  /* Arithmetic */
  INSTR_TYPE_ADC=21,        /* Add with carry */
  INSTR_TYPE_SBC=22,        /* Subtract with carry */
  INSTR_TYPE_CMP=23,        /* Compare to accumulator */
  INSTR_TYPE_CPX=24,        /* Compare to X register */
  INSTR_TYPE_CPY=25,        /* Compare to Y register */
  /* Increments & Decrements */
  INSTR_TYPE_INC=26,        /* Increment memory */
  INSTR_TYPE_INX=27,        /* Increment X register */
  INSTR_TYPE_INY=28,        /* Increment Y register */
  INSTR_TYPE_DEC=29,        /* Decrement memory */
  INSTR_TYPE_DEX=30,        /* Decrement X register */
  INSTR_TYPE_DEY=31,        /* Decrement Y register */
  /* Shifts */
  INSTR_TYPE_ASL=32,        /* Arithmetic shift left */
  INSTR_TYPE_LSR=33,        /* Logical shift right */
  INSTR_TYPE_ROL=34,        /* Rotate left */
  INSTR_TYPE_ROR=35,        /* Rotate right */
  /* Jumps & calls */
  INSTR_TYPE_JMP=36,        /* Jump to address */
  INSTR_TYPE_JSR=37,        /* Jump to subroutine */
  INSTR_TYPE_RTS=38,        /* Return from subroutine */
  /* Branches */
  INSTR_TYPE_BCC=39,        /* Branch on carry clear */
  INSTR_TYPE_BCS=40,        /* Branch on carry set */
  INSTR_TYPE_BEQ=41,        /* Branch on zero set */
  INSTR_TYPE_BMI=42,        /* Branch on result minus */
  INSTR_TYPE_BNE=43,        /* Branch on zero clear */
  INSTR_TYPE_BPL=44,        /* Branch on result positive */
  INSTR_TYPE_BVC=45,        /* Branch on overflow clear */
  INSTR_TYPE_BVS=46,        /* Branch on overflow set */
  /* Status flag changes */
  INSTR_TYPE_CLC=47,        /* Clear carry flag */
  INSTR_TYPE_CLD=48,        /* Clear decimal flag */
  INSTR_TYPE_CLI=49,        /* Clear interrupt disable flag */
  INSTR_TYPE_CLV=50,        /* Clear overflow flag */
  INSTR_TYPE_SEC=51,        /* Set carry flag */
  INSTR_TYPE_SED=52,        /* Set decimal flag */
  INSTR_TYPE_SEI=53,        /* Set interrupt disable flag */
  /* System functions */
  INSTR_TYPE_BRK=54,        /* Force interrupt */
  INSTR_TYPE_NOP=55,        /* No operation */
  INSTR_TYPE_RTI=56,        /* Return from interrupt */
  /* None */
  INSTR_TYPE_NONE=47,       /* Empty space in instruction set */
};

/* 6503 CPU structure */
struct cpu6502 {
  uint16_t pc;          /* Program counter */
  uint8_t sp;           /* Stack pointer = 0x0100 | sp */
  uint8_t a;            /* Accumulator */
  uint8_t x;            /* Index register X */
  uint8_t y;            /* Index register Y */
  union {
    uint8_t status;     /* Status register */
    struct {
      uint8_t c : 1;    /* Carry flag */
      uint8_t z : 1;    /* Zero flag */
      uint8_t i : 1;    /* Interrupt disable flag */
      uint8_t d : 1;    /* Decimal flag */
      uint8_t b : 1;    /* Break flag */
      uint8_t u : 1;    /* Unused flag */
      uint8_t v : 1;    /* Overflow flag */
      uint8_t n : 1;    /* Negative flag */
    } flags;            /* Status register flags */
  };
  uint8_t ram[RAM_SIZE];/* RAM */
  size_t ram_size;      /* Size of RAM, in bytes */
  size_t cycles_behind; /* Number of cycles the CPU is behind */
  /* The current instruction mode */
  enum addressing_modes_6502 instruction_mode;
  /* The current data */
  uint8_t data;
};

/* Addressing modes for each instruction */
static enum addressing_modes_6502 instruction_modes_6502[256] = {
  [0x00] = ADDR_MODE_IMPLIED,
  [0x01] = ADDR_MODE_INDIRECT_X,
  [0x02] = ADDR_MODE_NONE,
  [0x03] = ADDR_MODE_NONE,
  [0x04] = ADDR_MODE_NONE,
  [0x05] = ADDR_MODE_ZERO_PAGE,
  [0x06] = ADDR_MODE_ZERO_PAGE,
  [0x07] = ADDR_MODE_NONE,
  [0x08] = ADDR_MODE_IMPLIED,
  [0x09] = ADDR_MODE_IMMEDIATE,
  [0x0a] = ADDR_MODE_ACCUMULATOR,
  [0x0b] = ADDR_MODE_NONE,
  [0x0c] = ADDR_MODE_NONE,
  [0x0d] = ADDR_MODE_ABSOLUTE,
  [0x0e] = ADDR_MODE_ABSOLUTE,
  [0x0f] = ADDR_MODE_NONE,

  [0x10] = ADDR_MODE_RELATIVE,
  [0x11] = ADDR_MODE_INDIRECT_Y,
  [0x12] = ADDR_MODE_NONE,
  [0x13] = ADDR_MODE_NONE,
  [0x14] = ADDR_MODE_NONE,
  [0x15] = ADDR_MODE_ZERO_PAGE_X,
  [0x16] = ADDR_MODE_ZERO_PAGE_X,
  [0x17] = ADDR_MODE_NONE,
  [0x18] = ADDR_MODE_IMPLIED,
  [0x19] = ADDR_MODE_ABSOLUTE_Y,
  [0x1a] = ADDR_MODE_NONE,
  [0x1b] = ADDR_MODE_NONE,
  [0x1c] = ADDR_MODE_NONE,
  [0x1d] = ADDR_MODE_ABSOLUTE_X,
  [0x1e] = ADDR_MODE_ABSOLUTE_X,
  [0x1f] = ADDR_MODE_NONE,

  [0x20] = ADDR_MODE_ABSOLUTE,
  [0x21] = ADDR_MODE_INDIRECT_X,
  [0x22] = ADDR_MODE_NONE,
  [0x23] = ADDR_MODE_NONE,
  [0x24] = ADDR_MODE_ZERO_PAGE,
  [0x25] = ADDR_MODE_ZERO_PAGE,
  [0x26] = ADDR_MODE_ZERO_PAGE,
  [0x27] = ADDR_MODE_NONE,
  [0x28] = ADDR_MODE_IMPLIED,
  [0x29] = ADDR_MODE_IMMEDIATE,
  [0x2a] = ADDR_MODE_ACCUMULATOR,
  [0x2b] = ADDR_MODE_NONE,
  [0x2c] = ADDR_MODE_ABSOLUTE,
  [0x2d] = ADDR_MODE_ABSOLUTE,
  [0x2e] = ADDR_MODE_ABSOLUTE,
  [0x2f] = ADDR_MODE_NONE,

  [0x30] = ADDR_MODE_RELATIVE,
  [0x31] = ADDR_MODE_INDIRECT_Y,
  [0x32] = ADDR_MODE_NONE,
  [0x33] = ADDR_MODE_NONE,
  [0x34] = ADDR_MODE_NONE,
  [0x35] = ADDR_MODE_ZERO_PAGE_X,
  [0x36] = ADDR_MODE_ZERO_PAGE_X,
  [0x37] = ADDR_MODE_NONE,
  [0x38] = ADDR_MODE_IMPLIED,
  [0x39] = ADDR_MODE_ABSOLUTE_Y,
  [0x3a] = ADDR_MODE_NONE,
  [0x3b] = ADDR_MODE_NONE,
  [0x3c] = ADDR_MODE_NONE,
  [0x3d] = ADDR_MODE_ABSOLUTE_X,
  [0x3e] = ADDR_MODE_ABSOLUTE_X,
  [0x3f] = ADDR_MODE_NONE,

  [0x40] = ADDR_MODE_IMPLIED,
  [0x41] = ADDR_MODE_INDIRECT_X,
  [0x42] = ADDR_MODE_NONE,
  [0x43] = ADDR_MODE_NONE,
  [0x44] = ADDR_MODE_NONE,
  [0x45] = ADDR_MODE_ZERO_PAGE,
  [0x46] = ADDR_MODE_ZERO_PAGE,
  [0x47] = ADDR_MODE_NONE,
  [0x48] = ADDR_MODE_IMPLIED,
  [0x49] = ADDR_MODE_IMMEDIATE,
  [0x4a] = ADDR_MODE_ACCUMULATOR,
  [0x4b] = ADDR_MODE_NONE,
  [0x4c] = ADDR_MODE_ABSOLUTE,
  [0x4d] = ADDR_MODE_ABSOLUTE,
  [0x4e] = ADDR_MODE_ABSOLUTE,
  [0x4f] = ADDR_MODE_NONE,

  [0x50] = ADDR_MODE_RELATIVE,
  [0x51] = ADDR_MODE_INDIRECT_Y,
  [0x52] = ADDR_MODE_NONE,
  [0x53] = ADDR_MODE_NONE,
  [0x54] = ADDR_MODE_NONE,
  [0x55] = ADDR_MODE_ZERO_PAGE_X,
  [0x56] = ADDR_MODE_ZERO_PAGE_X,
  [0x57] = ADDR_MODE_NONE,
  [0x58] = ADDR_MODE_IMPLIED,
  [0x59] = ADDR_MODE_ABSOLUTE_Y,
  [0x5a] = ADDR_MODE_NONE,
  [0x5b] = ADDR_MODE_NONE,
  [0x5c] = ADDR_MODE_NONE,
  [0x5d] = ADDR_MODE_ABSOLUTE_X,
  [0x5e] = ADDR_MODE_ABSOLUTE_X,
  [0x5f] = ADDR_MODE_NONE,

  [0x60] = ADDR_MODE_IMPLIED,
  [0x61] = ADDR_MODE_INDIRECT_X,
  [0x62] = ADDR_MODE_NONE,
  [0x63] = ADDR_MODE_NONE,
  [0x64] = ADDR_MODE_NONE,
  [0x65] = ADDR_MODE_ZERO_PAGE,
  [0x66] = ADDR_MODE_ZERO_PAGE,
  [0x67] = ADDR_MODE_NONE,
  [0x68] = ADDR_MODE_IMPLIED,
  [0x69] = ADDR_MODE_IMMEDIATE,
  [0x6a] = ADDR_MODE_ACCUMULATOR,
  [0x6b] = ADDR_MODE_NONE,
  [0x6c] = ADDR_MODE_INDIRECT,
  [0x6d] = ADDR_MODE_ABSOLUTE,
  [0x6e] = ADDR_MODE_ABSOLUTE,
  [0x6f] = ADDR_MODE_NONE,

  [0x70] = ADDR_MODE_RELATIVE,
  [0x71] = ADDR_MODE_INDIRECT_Y,
  [0x72] = ADDR_MODE_NONE,
  [0x73] = ADDR_MODE_NONE,
  [0x74] = ADDR_MODE_NONE,
  [0x75] = ADDR_MODE_ZERO_PAGE_X,
  [0x76] = ADDR_MODE_ZERO_PAGE_X,
  [0x77] = ADDR_MODE_NONE,
  [0x78] = ADDR_MODE_IMPLIED,
  [0x79] = ADDR_MODE_ABSOLUTE_Y,
  [0x7a] = ADDR_MODE_NONE,
  [0x7b] = ADDR_MODE_NONE,
  [0x7c] = ADDR_MODE_NONE,
  [0x7d] = ADDR_MODE_ABSOLUTE_X,
  [0x7e] = ADDR_MODE_ABSOLUTE_X,
  [0x7f] = ADDR_MODE_NONE,

  [0x80] = ADDR_MODE_NONE,
  [0x81] = ADDR_MODE_INDIRECT_X,
  [0x82] = ADDR_MODE_NONE,
  [0x83] = ADDR_MODE_NONE,
  [0x84] = ADDR_MODE_ZERO_PAGE,
  [0x85] = ADDR_MODE_ZERO_PAGE,
  [0x86] = ADDR_MODE_ZERO_PAGE,
  [0x87] = ADDR_MODE_NONE,
  [0x88] = ADDR_MODE_IMPLIED,
  [0x89] = ADDR_MODE_NONE,
  [0x8a] = ADDR_MODE_IMPLIED,
  [0x8b] = ADDR_MODE_NONE,
  [0x8c] = ADDR_MODE_ABSOLUTE,
  [0x8d] = ADDR_MODE_ABSOLUTE,
  [0x8e] = ADDR_MODE_ABSOLUTE,
  [0x8f] = ADDR_MODE_NONE,

  [0x90] = ADDR_MODE_RELATIVE,
  [0x91] = ADDR_MODE_INDIRECT_Y,
  [0x92] = ADDR_MODE_NONE,
  [0x93] = ADDR_MODE_NONE,
  [0x94] = ADDR_MODE_ZERO_PAGE_X,
  [0x95] = ADDR_MODE_ZERO_PAGE_X,
  [0x96] = ADDR_MODE_ZERO_PAGE_Y,
  [0x97] = ADDR_MODE_NONE,
  [0x98] = ADDR_MODE_IMPLIED,
  [0x99] = ADDR_MODE_ABSOLUTE_Y,
  [0x9a] = ADDR_MODE_IMPLIED,
  [0x9b] = ADDR_MODE_NONE,
  [0x9c] = ADDR_MODE_NONE,
  [0x9d] = ADDR_MODE_ABSOLUTE_X,
  [0x9e] = ADDR_MODE_NONE,
  [0x9f] = ADDR_MODE_NONE,

  [0xa0] = ADDR_MODE_IMMEDIATE,
  [0xa1] = ADDR_MODE_INDIRECT_X,
  [0xa2] = ADDR_MODE_IMMEDIATE,
  [0xa3] = ADDR_MODE_NONE,
  [0xa4] = ADDR_MODE_ZERO_PAGE,
  [0xa5] = ADDR_MODE_ZERO_PAGE,
  [0xa6] = ADDR_MODE_ZERO_PAGE,
  [0xa7] = ADDR_MODE_NONE,
  [0xa8] = ADDR_MODE_IMPLIED,
  [0xa9] = ADDR_MODE_IMMEDIATE,
  [0xaa] = ADDR_MODE_IMPLIED,
  [0xab] = ADDR_MODE_NONE,
  [0xac] = ADDR_MODE_ABSOLUTE,
  [0xad] = ADDR_MODE_ABSOLUTE,
  [0xae] = ADDR_MODE_ABSOLUTE,
  [0xaf] = ADDR_MODE_NONE,

  [0xb0] = ADDR_MODE_RELATIVE,
  [0xb1] = ADDR_MODE_INDIRECT_Y,
  [0xb2] = ADDR_MODE_NONE,
  [0xb3] = ADDR_MODE_NONE,
  [0xb4] = ADDR_MODE_ZERO_PAGE_X,
  [0xb5] = ADDR_MODE_ZERO_PAGE_X,
  [0xb6] = ADDR_MODE_ZERO_PAGE_Y,
  [0xb7] = ADDR_MODE_NONE,
  [0xb8] = ADDR_MODE_IMPLIED,
  [0xb9] = ADDR_MODE_ABSOLUTE_Y,
  [0xba] = ADDR_MODE_IMPLIED,
  [0xbb] = ADDR_MODE_NONE,
  [0xbc] = ADDR_MODE_ABSOLUTE_X,
  [0xbd] = ADDR_MODE_ABSOLUTE_X,
  [0xbe] = ADDR_MODE_ABSOLUTE_Y,
  [0xbf] = ADDR_MODE_NONE,

  [0xc0] = ADDR_MODE_IMMEDIATE,
  [0xc1] = ADDR_MODE_INDIRECT_X,
  [0xc2] = ADDR_MODE_NONE,
  [0xc3] = ADDR_MODE_NONE,
  [0xc4] = ADDR_MODE_ZERO_PAGE,
  [0xc5] = ADDR_MODE_ZERO_PAGE,
  [0xc6] = ADDR_MODE_ZERO_PAGE,
  [0xc7] = ADDR_MODE_NONE,
  [0xc8] = ADDR_MODE_IMPLIED,
  [0xc9] = ADDR_MODE_IMMEDIATE,
  [0xca] = ADDR_MODE_IMPLIED,
  [0xcb] = ADDR_MODE_NONE,
  [0xcc] = ADDR_MODE_ABSOLUTE,
  [0xcd] = ADDR_MODE_ABSOLUTE,
  [0xce] = ADDR_MODE_ABSOLUTE,
  [0xcf] = ADDR_MODE_NONE,

  [0xd0] = ADDR_MODE_RELATIVE,
  [0xd1] = ADDR_MODE_INDIRECT_Y,
  [0xd2] = ADDR_MODE_NONE,
  [0xd3] = ADDR_MODE_NONE,
  [0xd4] = ADDR_MODE_NONE,
  [0xd5] = ADDR_MODE_ZERO_PAGE_X,
  [0xd6] = ADDR_MODE_ZERO_PAGE_Y,
  [0xd7] = ADDR_MODE_NONE,
  [0xd8] = ADDR_MODE_IMPLIED,
  [0xd9] = ADDR_MODE_ABSOLUTE_Y,
  [0xda] = ADDR_MODE_NONE,
  [0xdb] = ADDR_MODE_NONE,
  [0xdc] = ADDR_MODE_NONE,
  [0xdd] = ADDR_MODE_ABSOLUTE_X,
  [0xde] = ADDR_MODE_ABSOLUTE_X,
  [0xdf] = ADDR_MODE_NONE,

  [0xe0] = ADDR_MODE_IMMEDIATE,
  [0xe1] = ADDR_MODE_INDIRECT_X,
  [0xe2] = ADDR_MODE_NONE,
  [0xe3] = ADDR_MODE_NONE,
  [0xe4] = ADDR_MODE_ZERO_PAGE,
  [0xe5] = ADDR_MODE_ZERO_PAGE,
  [0xe6] = ADDR_MODE_ZERO_PAGE,
  [0xe7] = ADDR_MODE_NONE,
  [0xe8] = ADDR_MODE_IMPLIED,
  [0xe9] = ADDR_MODE_IMMEDIATE,
  [0xea] = ADDR_MODE_IMPLIED,
  [0xeb] = ADDR_MODE_NONE,
  [0xec] = ADDR_MODE_ABSOLUTE,
  [0xed] = ADDR_MODE_ABSOLUTE,
  [0xee] = ADDR_MODE_ABSOLUTE,
  [0xef] = ADDR_MODE_NONE,

  [0xf0] = ADDR_MODE_RELATIVE,
  [0xf1] = ADDR_MODE_INDIRECT_Y,
  [0xf2] = ADDR_MODE_NONE,
  [0xf3] = ADDR_MODE_NONE,
  [0xf4] = ADDR_MODE_NONE,
  [0xf5] = ADDR_MODE_ZERO_PAGE_X,
  [0xf6] = ADDR_MODE_ZERO_PAGE_X,
  [0xf7] = ADDR_MODE_NONE,
  [0xf8] = ADDR_MODE_IMPLIED,
  [0xf9] = ADDR_MODE_ABSOLUTE_Y,
  [0xfa] = ADDR_MODE_NONE,
  [0xfb] = ADDR_MODE_NONE,
  [0xfc] = ADDR_MODE_NONE,
  [0xfd] = ADDR_MODE_ABSOLUTE_X,
  [0xfe] = ADDR_MODE_ABSOLUTE_X,
  [0xff] = ADDR_MODE_NONE
};

static enum instr_types_6502 instruction_types_6502[256] = {
  [0x00] = INSTR_TYPE_BRK,
  [0x01] = INSTR_TYPE_ORA,
  [0x02] = INSTR_TYPE_NONE,
  [0x03] = INSTR_TYPE_NONE,
  [0x04] = INSTR_TYPE_NONE,
  [0x05] = INSTR_TYPE_ORA,
  [0x06] = INSTR_TYPE_ASL,
  [0x07] = INSTR_TYPE_NONE,
  [0x08] = INSTR_TYPE_PHP,
  [0x09] = INSTR_TYPE_ORA,
  [0x0a] = INSTR_TYPE_ASL,
  [0x0b] = INSTR_TYPE_NONE,
  [0x0c] = INSTR_TYPE_NONE,
  [0x0d] = INSTR_TYPE_ORA,
  [0x0e] = INSTR_TYPE_ASL,
  [0x0f] = INSTR_TYPE_NONE,

  [0x10] = INSTR_TYPE_BPL,
  [0x11] = INSTR_TYPE_ORA,
  [0x12] = INSTR_TYPE_NONE,
  [0x13] = INSTR_TYPE_NONE,
  [0x14] = INSTR_TYPE_NONE,
  [0x15] = INSTR_TYPE_ORA,
  [0x16] = INSTR_TYPE_ASL,
  [0x17] = INSTR_TYPE_NONE,
  [0x18] = INSTR_TYPE_CLC,
  [0x19] = INSTR_TYPE_ORA,
  [0x1a] = INSTR_TYPE_NONE,
  [0x1b] = INSTR_TYPE_NONE,
  [0x1c] = INSTR_TYPE_NONE,
  [0x1d] = INSTR_TYPE_ORA,
  [0x1e] = INSTR_TYPE_ASL,
  [0x1f] = INSTR_TYPE_NONE,

  [0x20] = INSTR_TYPE_JSR,
  [0x21] = INSTR_TYPE_AND,
  [0x22] = INSTR_TYPE_NONE,
  [0x23] = INSTR_TYPE_NONE,
  [0x24] = INSTR_TYPE_BIT,
  [0x25] = INSTR_TYPE_AND,
  [0x26] = INSTR_TYPE_ROL,
  [0x27] = INSTR_TYPE_NONE,
  [0x28] = INSTR_TYPE_PLP,
  [0x29] = INSTR_TYPE_AND,
  [0x2a] = INSTR_TYPE_ROL,
  [0x2b] = INSTR_TYPE_NONE,
  [0x2c] = INSTR_TYPE_BIT,
  [0x2d] = INSTR_TYPE_AND,
  [0x2e] = INSTR_TYPE_ROL,
  [0x2f] = INSTR_TYPE_NONE,

  [0x30] = INSTR_TYPE_BMI,
  [0x31] = INSTR_TYPE_AND,
  [0x32] = INSTR_TYPE_NONE,
  [0x33] = INSTR_TYPE_NONE,
  [0x34] = INSTR_TYPE_NONE,
  [0x35] = INSTR_TYPE_AND,
  [0x36] = INSTR_TYPE_ROL,
  [0x37] = INSTR_TYPE_NONE,
  [0x38] = INSTR_TYPE_SEC,
  [0x39] = INSTR_TYPE_AND,
  [0x3a] = INSTR_TYPE_NONE,
  [0x3b] = INSTR_TYPE_NONE,
  [0x3c] = INSTR_TYPE_NONE,
  [0x3d] = INSTR_TYPE_AND,
  [0x3e] = INSTR_TYPE_ROL,
  [0x3f] = INSTR_TYPE_NONE,

  [0x40] = INSTR_TYPE_RTI,
  [0x41] = INSTR_TYPE_EOR,
  [0x42] = INSTR_TYPE_NONE,
  [0x43] = INSTR_TYPE_NONE,
  [0x44] = INSTR_TYPE_NONE,
  [0x45] = INSTR_TYPE_EOR,
  [0x46] = INSTR_TYPE_LSR,
  [0x47] = INSTR_TYPE_NONE,
  [0x48] = INSTR_TYPE_PHA,
  [0x49] = INSTR_TYPE_EOR,
  [0x4a] = INSTR_TYPE_LSR,
  [0x4b] = INSTR_TYPE_NONE,
  [0x4c] = INSTR_TYPE_JMP,
  [0x4d] = INSTR_TYPE_EOR,
  [0x4e] = INSTR_TYPE_LSR,
  [0x4f] = INSTR_TYPE_NONE,

  [0x50] = INSTR_TYPE_BVC,
  [0x51] = INSTR_TYPE_EOR,
  [0x52] = INSTR_TYPE_NONE,
  [0x53] = INSTR_TYPE_NONE,
  [0x54] = INSTR_TYPE_NONE,
  [0x55] = INSTR_TYPE_EOR,
  [0x56] = INSTR_TYPE_LSR,
  [0x57] = INSTR_TYPE_NONE,
  [0x58] = INSTR_TYPE_CLI,
  [0x59] = INSTR_TYPE_EOR,
  [0x5a] = INSTR_TYPE_NONE,
  [0x5b] = INSTR_TYPE_NONE,
  [0x5c] = INSTR_TYPE_NONE,
  [0x5d] = INSTR_TYPE_EOR,
  [0x5e] = INSTR_TYPE_LSR,
  [0x5f] = INSTR_TYPE_NONE,

  [0x60] = INSTR_TYPE_RTS,
  [0x61] = INSTR_TYPE_ADC,
  [0x62] = INSTR_TYPE_NONE,
  [0x63] = INSTR_TYPE_NONE,
  [0x64] = INSTR_TYPE_NONE,
  [0x65] = INSTR_TYPE_ADC,
  [0x66] = INSTR_TYPE_ROR,
  [0x67] = INSTR_TYPE_NONE,
  [0x68] = INSTR_TYPE_PLA,
  [0x69] = INSTR_TYPE_ADC,
  [0x6a] = INSTR_TYPE_ROR,
  [0x6b] = INSTR_TYPE_NONE,
  [0x6c] = INSTR_TYPE_JMP,
  [0x6d] = INSTR_TYPE_ADC,
  [0x6e] = INSTR_TYPE_ROR,
  [0x6f] = INSTR_TYPE_NONE,
  
  [0x70] = INSTR_TYPE_BVS,
  [0x71] = INSTR_TYPE_ADC,
  [0x72] = INSTR_TYPE_NONE,
  [0x73] = INSTR_TYPE_NONE,
  [0x74] = INSTR_TYPE_NONE,
  [0x75] = INSTR_TYPE_ADC,
  [0x76] = INSTR_TYPE_ROR,
  [0x77] = INSTR_TYPE_NONE,
  [0x78] = INSTR_TYPE_SEI,
  [0x79] = INSTR_TYPE_ADC,
  [0x7a] = INSTR_TYPE_NONE,
  [0x7b] = INSTR_TYPE_NONE,
  [0x7c] = INSTR_TYPE_NONE,
  [0x7d] = INSTR_TYPE_ADC,
  [0x7e] = INSTR_TYPE_ROR,
  [0x7f] = INSTR_TYPE_NONE,

  [0x80] = INSTR_TYPE_NONE,
  [0x81] = INSTR_TYPE_STA,
  [0x82] = INSTR_TYPE_NONE,
  [0x83] = INSTR_TYPE_NONE,
  [0x84] = INSTR_TYPE_STY,
  [0x85] = INSTR_TYPE_STA,
  [0x86] = INSTR_TYPE_STX,
  [0x87] = INSTR_TYPE_NONE,
  [0x88] = INSTR_TYPE_DEY,
  [0x89] = INSTR_TYPE_NONE,
  [0x8a] = INSTR_TYPE_TXA,
  [0x8b] = INSTR_TYPE_NONE,
  [0x8c] = INSTR_TYPE_STY,
  [0x8d] = INSTR_TYPE_STA,
  [0x8e] = INSTR_TYPE_STX,
  [0x8f] = INSTR_TYPE_NONE,

  [0x90] = INSTR_TYPE_BCC,
  [0x91] = INSTR_TYPE_STA,
  [0x92] = INSTR_TYPE_NONE,
  [0x93] = INSTR_TYPE_NONE,
  [0x94] = INSTR_TYPE_STY,
  [0x95] = INSTR_TYPE_STA,
  [0x96] = INSTR_TYPE_STX,
  [0x97] = INSTR_TYPE_NONE,
  [0x98] = INSTR_TYPE_TYA,
  [0x99] = INSTR_TYPE_STA,
  [0x9a] = INSTR_TYPE_TXS,
  [0x9b] = INSTR_TYPE_NONE,
  [0x9c] = INSTR_TYPE_NONE,
  [0x9d] = INSTR_TYPE_STA,
  [0x9e] = INSTR_TYPE_NONE,
  [0x9f] = INSTR_TYPE_NONE,

  [0xa0] = INSTR_TYPE_LDY,
  [0xa1] = INSTR_TYPE_LDA,
  [0xa2] = INSTR_TYPE_LDX,
  [0xa3] = INSTR_TYPE_NONE,
  [0xa4] = INSTR_TYPE_LDY,
  [0xa5] = INSTR_TYPE_LDA,
  [0xa6] = INSTR_TYPE_LDX,
  [0xa7] = INSTR_TYPE_NONE,
  [0xa8] = INSTR_TYPE_TAY,
  [0xa9] = INSTR_TYPE_LDA,
  [0xaa] = INSTR_TYPE_TAX,
  [0xab] = INSTR_TYPE_NONE,
  [0xac] = INSTR_TYPE_LDY,
  [0xad] = INSTR_TYPE_LDA,
  [0xae] = INSTR_TYPE_LDX,
  [0xaf] = INSTR_TYPE_NONE,

  [0xb0] = INSTR_TYPE_BCS,
  [0xb1] = INSTR_TYPE_LDA,
  [0xb2] = INSTR_TYPE_NONE,
  [0xb3] = INSTR_TYPE_NONE,
  [0xb4] = INSTR_TYPE_LDY,
  [0xb5] = INSTR_TYPE_LDA,
  [0xb6] = INSTR_TYPE_LDX,
  [0xb7] = INSTR_TYPE_NONE,
  [0xb8] = INSTR_TYPE_CLV,
  [0xb9] = INSTR_TYPE_LDA,
  [0xba] = INSTR_TYPE_TSX,
  [0xbb] = INSTR_TYPE_NONE,
  [0xbc] = INSTR_TYPE_LDY,
  [0xbd] = INSTR_TYPE_LDA,
  [0xbe] = INSTR_TYPE_LDX,
  [0xbf] = INSTR_TYPE_NONE,

  [0xc0] = INSTR_TYPE_CPY,
  [0xc1] = INSTR_TYPE_CMP,
  [0xc2] = INSTR_TYPE_NONE,
  [0xc3] = INSTR_TYPE_NONE,
  [0xc4] = INSTR_TYPE_CPY,
  [0xc5] = INSTR_TYPE_CMP,
  [0xc6] = INSTR_TYPE_DEC,
  [0xc7] = INSTR_TYPE_NONE,
  [0xc8] = INSTR_TYPE_INY,
  [0xc9] = INSTR_TYPE_CMP,
  [0xca] = INSTR_TYPE_DEX,
  [0xcb] = INSTR_TYPE_NONE,
  [0xcc] = INSTR_TYPE_CPY,
  [0xcd] = INSTR_TYPE_CMP,
  [0xce] = INSTR_TYPE_DEC,
  [0xcf] = INSTR_TYPE_NONE,

  [0xd0] = INSTR_TYPE_BNE,
  [0xd1] = INSTR_TYPE_CMP,
  [0xd2] = INSTR_TYPE_NONE,
  [0xd3] = INSTR_TYPE_NONE,
  [0xd4] = INSTR_TYPE_NONE,
  [0xd5] = INSTR_TYPE_CMP,
  [0xd6] = INSTR_TYPE_DEC,
  [0xd7] = INSTR_TYPE_NONE,
  [0xd8] = INSTR_TYPE_CLD,
  [0xd9] = INSTR_TYPE_CMP,
  [0xda] = INSTR_TYPE_NONE,
  [0xdb] = INSTR_TYPE_NONE,
  [0xdc] = INSTR_TYPE_NONE,
  [0xdd] = INSTR_TYPE_CMP,
  [0xde] = INSTR_TYPE_DEC,
  [0xdf] = INSTR_TYPE_NONE,

  [0xe0] = INSTR_TYPE_CPX,
  [0xe1] = INSTR_TYPE_SBC,
  [0xe2] = INSTR_TYPE_NONE,
  [0xe3] = INSTR_TYPE_NONE,
  [0xe4] = INSTR_TYPE_CPX,
  [0xe5] = INSTR_TYPE_SBC,
  [0xe6] = INSTR_TYPE_INC,
  [0xe7] = INSTR_TYPE_NONE,
  [0xe8] = INSTR_TYPE_INX,
  [0xe9] = INSTR_TYPE_SBC,
  [0xea] = INSTR_TYPE_NOP,
  [0xeb] = INSTR_TYPE_NONE,
  [0xec] = INSTR_TYPE_CPX,
  [0xed] = INSTR_TYPE_SBC,
  [0xee] = INSTR_TYPE_INC,
  [0xef] = INSTR_TYPE_NONE,

  [0xf0] = INSTR_TYPE_BEQ,
  [0xf1] = INSTR_TYPE_SBC,
  [0xf2] = INSTR_TYPE_NONE,
  [0xf3] = INSTR_TYPE_NONE,
  [0xf4] = INSTR_TYPE_NONE,
  [0xf5] = INSTR_TYPE_SBC,
  [0xf6] = INSTR_TYPE_INC,
  [0xf7] = INSTR_TYPE_NONE,
  [0xf8] = INSTR_TYPE_SED,
  [0xf9] = INSTR_TYPE_SBC,
  [0xfa] = INSTR_TYPE_NONE,
  [0xfb] = INSTR_TYPE_NONE,
  [0xfc] = INSTR_TYPE_NONE,
  [0xfd] = INSTR_TYPE_SBC,
  [0xfe] = INSTR_TYPE_INC,
  [0xff] = INSTR_TYPE_NONE
};

/* Reset the 6502 CPU */
static void cpu6502_reset(struct cpu6502 *cpu) {
  /* Resetting takes 6 cycles, according to wikipedia */
  cpu->cycles_behind = 6;
  /* The size of the RAM */
  cpu->ram_size = RAM_SIZE;
  /* Chip state guaranteed */
  /* --- THIS SEEMS TO BE WHAT CHIPS ALWAYS DO --- */
  cpu->flags.i = 1; /* Set interrupt disable */
  cpu->flags.d = 0; /* Clear decimal (this isn't guaranteed on every 6502) */
  /* The program counter set to the value at 0xfffc-0xfffd (the reset vector) */
  cpu->pc = cpu->ram[RESET_VECTOR] | (cpu->ram[RESET_VECTOR+1] << 8);
  /* --- I MIGHT AS WELL ALSO DO THIS --- */
  cpu->flags.z = 1; /* Set zero */
  cpu->flags.n = 0; /* Clear negative */
  cpu->flags.v = 0; /* Clear overflow */
  cpu->flags.c = 0; /* Clear carry */
  cpu->sp = 0xff;   /* Set stack pointer to top of range allowed */
  cpu->a = 0;       /* Clear accumulator */
  cpu->x = 0;       /* Clear index register X */
  cpu->y = 0;       /* Clear index register Y */
}
/* Step the 6502 CPU */
static void cpu6502_step(struct cpu6502 *cpu) {
  if (cpu->cycles_behind > 0) {
  }
  cpu->cycles_behind--;
}

#endif /* CPU6502_H */
