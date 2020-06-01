# system\e
System\e is a virtual machine inspired by old mainframes and Lisp machines and written in C++.

## Specification:
System\e simulates a word addressable, memory managed, , tagged architecture system. The use of tagged pointers allows the machine to store data regarding the contents of a pointer alongside the pointer itself, allowing for garbage collection and typechecking to be conducted by the opcodes themselves.

### Basic Info:
| Field                      | Data                                            |
|----------------------------|-------------------------------------------------|
| Addressing Scheme          | word addressed                                  |
| Endianess                  | little endian                                   |
| Word Size                  | 48 Bits (32 MSB data, 8 type, 8 LSB references) |
| Page Size                  | 256 Words                                       |
| Memory Size                | 327680 words, 4096 pages                        |
| Addressable Virtual Memory | 268435456 words, 1048576 pages                  |

### Types:
| binary    | mnemonic | description                                                                                  |
|-----------|----------|----------------------------------------------------------------------------------------------|
| 0000 0000 | ANY      | Any type  (data can be manipulated by all instructions without causing type error interrupt) |
| 0000 0001 | BL       | boolean                                                                                      |
| 0000 0010 | ZL       | integer                                                                                      |
| 0000 0011 | RE       | real                                                                                         |
| 0000 0100 | CH       | character                                                                                    |
| 0000 0101 | SY       | symbol                                                                                       |
| 0000 0110 | KW       | keyword                                                                                      |
| 0000 0111 | CS       | cons  cell                                                                                   |
| 0000 1000 | VC       | vector                                                                                       |
| 0000 1001 | ST       | string                                                                                       |
| 0000 1011 | FN       | function                                                                                     |
| 0000 1100 | IN       | instruction                                                                                  |
| 0000 1001 | TP       | type                                                                                         |
| 0000 1010 | FN       | function                                                                                     |
|           |          | ... user defined types ...                                                                   |

### Registers:
#### 48 bit registers:

##### Arithmetic:
| binary | mnemonic | description                                                                  |
|--------|----------|------------------------------------------------------------------------------|
| 0000   | A        | accumulator         (register operation is applied to it as second argument) |
| 0001   | S        | secondary register  (for two operand instructions)                           |
| 0010   | T        | tertiary register   (for two operand instructions)                           |
| 0011   | Q        | quaternary register (for two operand instructions)                           |
##### Special:
| binary | mnemonic | description        |
|--------|----------|--------------------|
| 0100   | EP       | execution pointer  |
| 0101   | SH       | stack head pointer |
| 0110   | SB       | stack base pointer |
#### 16 bit registers:
| binary | mnemonic | description        |
|--------|----------|--------------------|
| binary | mnemonic | description        |
| 0111   | IR       | interrupt register |
| 1111   | FL       | flag register      |

#### Flags:
Flags are located in the FL register and may be written to by MSR instructions, however use of this feature is not advised as it is rather unintuitive.
| offset | mnemonic | description                                                                         |
|--------|----------|-------------------------------------------------------------------------------------|
| 00     | TF       | truth flag, high if last boolean statement evaluated was true, else low, starts low |
| 01     | OF       | overflow flag, high if last instruction was an overflow, starts low                 |
| 02     | CF       | carry flag, high if last instruction caused carry, starts low                       |
| 03     | ZF       | zero flag, high of A is zero, starts low                                            |
| 04     | NF       | negative flag, high if A is negative, starts low                                    |
| 05     | TC       | type checking flag, if high typechecking occurs, starts high                        |
| 06     | IF       | interrupt enable flag, high if interrupts are enabled, starts high                  |
| 07     | VF       | vector flag, high if vector checking is enabled, starts high                        |
| 08     | PF       | parity flag, high if even, low if odd, starts low                                   |
| 09     | AM       | addressing mode, high if direct, low if indirect, starts high                       |

#### Interrupts:
Interrupt handlers are designated in the assembler by a tag labeled with the interrupt name and is limited to 256 instructions in length for each interrupt as the interrupt handlers are on special pages starting on page 0x10000000, at the end of the virtual memory of the machine. The interrupt register may be written to to artificially execute an interrupt.
| binary | mnemonic | description                                      |
|--------|----------|--------------------------------------------------|
| 0000   | HF       | hardware failure                                 |
| 0001   | TE       | type error                                       |
| 0010   | PE       | parity error                                     |
| 0011   | VO       | vector out of bounds error                       |
| 0100   | SO       | string out of bounds error                       |
| 0101   | CD       | core dump interrupt                              |
| 0110   | PE       | program end interrupt                            |
| 0111   | RI       | reference exceeded interrupt                     |
|        |          | ... ascii character codes for keyboard input ... |

### Instructions:
All one argument instructions operate on a regular register and A, the accumulator. All instructions also conduct type checking unless the TC flag is low. Instruction operands are eight bits in length with the remaining 40 bits being used the two operands. Operands are specified by their offset from EP and thus instructions may only directly access data located 1048576 words away from themselves in direct addressing mode. Thus indirect addressing mode is recommended.
| binary  | mnemonic types | description                                 |
|---------|----------------|---------------------------------------------|
| 00 0001 | MR ANY, ANY    | move from src (memory)   to dest (register) |
| 00 0010 | MM ANY, ANY    | move from src (register) to dest (memory)   |
| 00 0011 | AD ZL          | addition                                    |
| 00 0100 | SB ZL          | subtraction                                 |
| 00 0101 | ML ZL          | multiplication                              |
| 00 0111 | DV ZL          | division                                    |
| 00 1000 | BRU IN         | branch unconditionally                      |
| 00 1001 | BRC IN         | branch only if TF is high                   |
| 00 1010 | LT ANY         | less than                                   |
| 00 1011 | GT ANY         | greater than                                |
| 00 1100 | NE ANY         | not equal                                   |
| 00 1101 | EQ ANY         | equal                                       |
| 00 1111 | NG BL          | negation                                    |
| 01 0000 | AND BL or ZL   | bitwise and                                 |
| 01 0001 | OR  BL or ZL   | bitwise or                                  |
| 01 0010 | XOR BL or ZL   | bitwise xor                                 |
| 01 0011 | NOT BL or ZL   | bitwise not                                 |
| 01 0100 | CALL FN        | call function                               |
| 01 0101 | CHTP TP        | change type of accumulator to TP            |
| 01 0110 | RAD RL         | real addition                               |
| 01 0111 | RSB RL         | real subtraction                            |
| 01 1000 | RML RL         | real multiplication                         |
| 01 1001 | RDV RL         | real division                               |
| 01 1010 | MSR ANY        | moves MSB of data into a 16 bit register    |
| 01 1011 | DAM            | sets the mode to direct addressing          |
| 01 1100 | IAM            | sets the mode to indirect addressing        |
| 01 1101 | CKTP TP        | checks the type of the accumulator          |



## Implementation:

### Word Type:
The word type is implemented as a struct of two chars and one integer. GNU C++ uses unsigned chars and thus the individual words are 48 bits long.

### Registers:
48 bit registers are implemented as variables containing the se_word struct.

### Memory:
Memory is divided into two types

### Accessing Data:
Data is accessed via.

### Microcode:
Microcode.
