	.file	"test_cpe.c"
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.text
.Ltext0:
	.type	setup, @function
setup:
.LFB19:
	.file 1 "test_cpe.c"
	.loc 1 12 0
.LVL0:
	.loc 1 13 0
	movq	$data, a(%rip)
	.loc 1 14 0
	movslq	%edi,%rdi
.LVL1:
	salq	$2, %rdi
.LVL2:
	leaq	data(%rdi), %rax
	movq	%rax, b(%rip)
	.loc 1 15 0
	leaq	(%rax,%rdi), %rdi
	movq	%rdi, c(%rip)
	.loc 1 16 0
	movq	%rax, p(%rip)
	.loc 1 17 0
	ret
.LFE19:
	.size	setup, .-setup
.globl psum1
	.type	psum1, @function
psum1:
.LFB20:
	.loc 1 22 0
.LVL3:
	.loc 1 24 0
	movl	(%rdi), %eax
	movl	%eax, (%rsi)
	.loc 1 25 0
	cmpq	$1, %rdx
	jle	.L6
	movl	$1, %eax
.LVL4:
.L5:
	.loc 1 26 0
	movss	-4(%rsi,%rax,4), %xmm0
	addss	(%rdi,%rax,4), %xmm0
	movss	%xmm0, (%rsi,%rax,4)
	.loc 1 25 0
	addq	$1, %rax
	cmpq	%rax, %rdx
	jg	.L5
.L6:
	rep
	ret
.LFE20:
	.size	psum1, .-psum1
.globl psum2
	.type	psum2, @function
psum2:
.LFB21:
	.loc 1 30 0
.LVL5:
	.loc 1 32 0
	movl	(%rdi), %eax
	movl	%eax, (%rsi)
	.loc 1 33 0
	leaq	-1(%rdx), %r8
	movl	$1, %eax
.LVL6:
	movl	$1, %ecx
.LVL7:
	cmpq	$1, %r8
	jle	.L10
.L14:
.LBB14:
	.loc 1 34 0
	movss	-4(%rsi,%rax,4), %xmm0
	addss	(%rdi,%rax,4), %xmm0
.LVL8:
	.loc 1 35 0
	movss	%xmm0, (%rsi,%rax,4)
	.loc 1 36 0
	addss	4(%rdi,%rax,4), %xmm0
.LVL9:
	movss	%xmm0, 4(%rsi,%rax,4)
.LBE14:
	.loc 1 33 0
	addq	$2, %rax
.LVL10:
	cmpq	%rax, %r8
	jg	.L14
	leaq	-3(%rdx), %rax
.LVL11:
	andq	$-2, %rax
	leaq	3(%rax), %rcx
.LVL12:
.L10:
	.loc 1 39 0
	cmpq	%rcx, %rdx
	jle	.L13
	.loc 1 40 0
	leaq	0(,%rcx,4), %rax
	movss	-4(%rsi,%rcx,4), %xmm0
	addss	(%rdi,%rax), %xmm0
	movss	%xmm0, (%rsi,%rax)
.L13:
	rep
	ret
.LFE21:
	.size	psum2, .-psum2
.globl psum1a
	.type	psum1a, @function
psum1a:
.LFB22:
	.loc 1 46 0
.LVL13:
	.loc 1 50 0
	movss	(%rdi), %xmm0
.LVL14:
	movss	%xmm0, (%rsi)
	.loc 1 51 0
	cmpq	$1, %rdx
	jle	.L19
	movl	$1, %eax
.LVL15:
.L18:
	.loc 1 52 0
	addss	(%rdi,%rax,4), %xmm0
.LVL16:
	.loc 1 53 0
	movss	%xmm0, (%rsi,%rax,4)
	.loc 1 51 0
	addq	$1, %rax
	cmpq	%rax, %rdx
	jg	.L18
.LVL17:
.L19:
	rep
	ret
.LFE22:
	.size	psum1a, .-psum1a
.globl tpsum1
	.type	tpsum1, @function
tpsum1:
.LFB23:
	.loc 1 60 0
.LVL18:
	.loc 1 61 0
	movslq	%edi,%rdx
.LVL19:
	movq	p(%rip), %rsi
	movq	a(%rip), %rdi
	call	psum1
.LVL20:
	.loc 1 62 0
	rep
	ret
.LFE23:
	.size	tpsum1, .-tpsum1
.globl tpsum2
	.type	tpsum2, @function
tpsum2:
.LFB24:
	.loc 1 65 0
.LVL21:
	.loc 1 66 0
	movslq	%edi,%rdx
.LVL22:
	movq	p(%rip), %rsi
	movq	a(%rip), %rdi
	call	psum2
.LVL23:
	.loc 1 67 0
	rep
	ret
.LFE24:
	.size	tpsum2, .-tpsum2
.globl tpsum1a
	.type	tpsum1a, @function
tpsum1a:
.LFB25:
	.loc 1 70 0
.LVL24:
	.loc 1 71 0
	movslq	%edi,%rdx
.LVL25:
	movq	p(%rip), %rsi
	movq	a(%rip), %rdi
	call	psum1a
.LVL26:
	.loc 1 72 0
	rep
	ret
.LFE25:
	.size	tpsum1a, .-tpsum1a
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"Usage: %s [-q] [-u] [-a][-r] [-l CNT] [-n REPS] [-s SAMPLES] [-b BIAS] [-k SEED]\n"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"\t-q\tQuiet mode"
.LC2:
	.string	"\t-u\tUse unrolled code"
.LC3:
	.string	"\t-u\tUse alternate code"
.LC4:
	.string	"\t-r\tUse random sampling"
.LC5:
	.string	"\t-l len\tSet vector length"
	.section	.rodata.str1.8
	.align 8
.LC6:
	.string	"\t-n reps\tSet number of repetitions"
	.align 8
.LC7:
	.string	"\t-s samples\tSet number of samples"
	.align 8
.LC8:
	.string	"\t-b bias\tSet bias (between 0 & 1.0) determining sample value"
	.section	.rodata.str1.1
.LC9:
	.string	"\t-k seed\tSet seed for rng"
	.text
	.type	usage, @function
usage:
.LFB27:
	.loc 1 99 0
.LVL27:
	subq	$8, %rsp
.LCFI0:
.LBB15:
.LBB16:
	.file 2 "/usr/include/bits/stdio2.h"
	.loc 2 105 0
	movq	%rdi, %rdx
	movl	$.LC0, %esi
	movl	$1, %edi
.LVL28:
	movl	$0, %eax
	call	__printf_chk
.LBE16:
.LBE15:
	.loc 1 101 0
	movl	$.LC1, %edi
	call	puts
	.loc 1 102 0
	movl	$.LC2, %edi
	call	puts
	.loc 1 103 0
	movl	$.LC3, %edi
	call	puts
	.loc 1 104 0
	movl	$.LC4, %edi
	call	puts
	.loc 1 105 0
	movl	$.LC5, %edi
	call	puts
	.loc 1 106 0
	movl	$.LC6, %edi
	call	puts
	.loc 1 107 0
	movl	$.LC7, %edi
	call	puts
	.loc 1 108 0
	movl	$.LC8, %edi
	call	puts
	.loc 1 109 0
	movl	$.LC9, %edi
	call	puts
	.loc 1 110 0
	movl	$0, %edi
	call	exit
.LFE27:
	.size	usage, .-usage
	.section	.rodata.str1.8
	.align 8
.LC10:
	.string	"Ref: i = %d, b[i-1] = %.0f, a[i] = %.0f, b[i] = %.0f.  Got b[i-1] = %.0f, b[i] = %.0f\n"
	.section	.rodata.str1.1
.LC11:
	.string	"Too many errors.  Exiting"
	.text
.globl check
	.type	check, @function
check:
.LFB26:
	.loc 1 75 0
.LVL29:
	pushq	%r15
.LCFI1:
	pushq	%r14
.LCFI2:
	pushq	%r13
.LCFI3:
	pushq	%r12
.LCFI4:
	pushq	%rbp
.LCFI5:
	pushq	%rbx
.LCFI6:
	subq	$8, %rsp
.LCFI7:
	movq	%rdi, %r15
	movl	%esi, %r13d
	.loc 1 78 0
	testl	%esi, %esi
	jle	.L29
.LVL30:
	movl	$0, %r12d
.LVL31:
	movl	$0, %ebp
	.loc 1 79 0
	movabsq	$2361183241434822607, %r14
.L30:
	movq	%rbp, %rbx
	addq	a(%rip), %rbx
	call	random
.LVL32:
	movq	%rax, %rsi
	imulq	%r14
	sarq	$7, %rdx
	movq	%rsi, %rcx
	sarq	$63, %rcx
	subq	%rcx, %rdx
	imulq	$1000, %rdx, %rdx
	movq	%rsi, %rax
	subq	%rdx, %rax
	cvtsi2ssq	%rax, %xmm0
	movss	%xmm0, (%rbx)
	.loc 1 78 0
	addl	$1, %r12d
	addq	$4, %rbp
	cmpl	%r12d, %r13d
	jg	.L30
	.loc 1 81 0
	movq	b(%rip), %rax
	movq	%rax, p(%rip)
	.loc 1 82 0
	movl	%r13d, %edi
	call	tpsum1
	.loc 1 83 0
	movq	c(%rip), %rax
	movq	%rax, p(%rip)
	.loc 1 84 0
	movl	%r13d, %edi
	call	*%r15
	movl	$0, %ebp
.LVL33:
	movl	$0, %r12d
.LVL34:
	movl	$0, %ebx
.L33:
	.loc 1 86 0
	movq	b(%rip), %rax
	movss	(%rax,%rbx), %xmm4
	movq	c(%rip), %rcx
	movss	(%rcx,%rbx), %xmm2
	ucomiss	%xmm2, %xmm4
	jp	.L36
	je	.L31
.L36:
	leaq	-4(%rbx), %rdx
.LBB17:
.LBB18:
	.loc 2 105 0
	unpcklps	%xmm4, %xmm4
	cvtps2pd	%xmm4, %xmm4
	movss	(%rax,%rdx), %xmm3
	cvtps2pd	%xmm3, %xmm3
	unpcklps	%xmm2, %xmm2
	cvtps2pd	%xmm2, %xmm2
	movq	a(%rip), %rax
	movss	(%rax,%rbx), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	(%rcx,%rdx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movl	%ebp, %edx
	movl	$.LC10, %esi
	movl	$1, %edi
	movl	$5, %eax
	call	__printf_chk
.LBE18:
.LBE17:
	.loc 1 89 0
	addl	$1, %r12d
	.loc 1 90 0
	cmpl	$10, %r12d
	jle	.L31
	.loc 1 91 0
	movl	$.LC11, %edi
	call	puts
	.loc 1 92 0
	movl	$1, %edi
	call	exit
.L31:
	.loc 1 85 0
	addl	$1, %ebp
	addq	$4, %rbx
	cmpl	%ebp, %r13d
	jg	.L33
	jmp	.L35
.LVL35:
.L29:
	.loc 1 81 0
	movq	b(%rip), %rax
	movq	%rax, p(%rip)
	.loc 1 82 0
	movl	%esi, %edi
	call	tpsum1
.LVL36:
	.loc 1 83 0
	movq	c(%rip), %rax
	movq	%rax, p(%rip)
	.loc 1 84 0
	movl	%r13d, %edi
	call	*%r15
.LVL37:
.L35:
	.loc 1 96 0
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
.LVL38:
	popq	%r12
.LVL39:
	popq	%r13
.LVL40:
	popq	%r14
	popq	%r15
.LVL41:
	ret
.LFE26:
	.size	check, .-check
	.section	.rodata.str1.1
.LC14:
	.string	"Cycles = %.2f, CPE = %.2f\n"
.LC15:
	.string	"Full cpe = %.2f\n"
.LC16:
	.string	"Default cpe = %.2f\n"
	.text
.globl main
	.type	main, @function
main:
.LFB28:
	.loc 1 114 0
.LVL42:
	pushq	%r15
.LCFI8:
	pushq	%r14
.LCFI9:
	pushq	%r13
.LCFI10:
	pushq	%r12
.LCFI11:
	pushq	%rbp
.LCFI12:
	pushq	%rbx
.LCFI13:
	subq	$24, %rsp
.LCFI14:
	movl	%edi, %r12d
	movq	%rsi, %rbp
	.loc 1 124 0
	cmpl	$1, %edi
	jle	.L41
.LVL43:
	movl	$tpsum1, %r14d
.LVL44:
	movl	$1024, %r13d
.LVL45:
	movl	$1, %ebx
.LVL46:
	movl	$8, %r15d
.LVL47:
	xorpd	%xmm0, %xmm0
	movsd	%xmm0, (%rsp)
.LVL48:
	movl	$1, 8(%rsp)
.LVL49:
	movl	$31415, 12(%rsp)
.LVL50:
	movl	$2, 16(%rsp)
.LVL51:
	movl	$0, 20(%rsp)
.LVL52:
.LVL53:
.L61:
	.loc 1 125 0
	movslq	%ebx,%rax
.LVL54:
	movq	(%rbp,%rax,8), %rax
.LVL55:
	cmpb	$45, (%rax)
	jne	.L42
	.loc 1 126 0
	movzbl	1(%rax), %eax
	subl	$97, %eax
	cmpb	$20, %al
	ja	.L43
	movzbl	%al, %eax
	jmp	*.L53(,%rax,8)
	.section	.rodata
	.align 8
	.align 4
.L53:
	.quad	.L44
	.quad	.L45
	.quad	.L43
	.quad	.L43
	.quad	.L43
	.quad	.L43
	.quad	.L43
	.quad	.L43
	.quad	.L43
	.quad	.L43
	.quad	.L46
	.quad	.L47
	.quad	.L43
	.quad	.L48
	.quad	.L43
	.quad	.L43
	.quad	.L49
	.quad	.L50
	.quad	.L51
	.quad	.L43
	.quad	.L52
	.text
.L44:
	movl	$tpsum1a, %r14d
	jmp	.L42
.L52:
	movl	$tpsum2, %r14d
	jmp	.L42
.L50:
	movl	$1, 20(%rsp)
.LVL56:
	jmp	.L42
.L49:
	.loc 1 135 0
	movl	$0, 16(%rsp)
.LVL57:
	jmp	.L42
.L47:
	.loc 1 140 0
	addl	$1, %ebx
.LVL58:
	.loc 1 141 0
	cmpl	%ebx, %r12d
	jg	.L54
	.loc 1 142 0
	movq	(%rbp), %rdi
	call	usage
.LVL59:
.L54:
	.loc 1 143 0
	movslq	%ebx,%rax
.LVL60:
	movq	(%rbp,%rax,8), %rdi
	call	atoi
.LVL61:
	movl	%eax, %r13d
	.loc 1 144 0
	leal	-1(%r13), %eax
	cmpl	$262143, %eax
	jbe	.L42
	.loc 1 145 0
	movq	(%rbp), %rdi
	call	usage
	jmp	.L42
.LVL62:
.L48:
	.loc 1 148 0
	addl	$1, %ebx
.LVL63:
	.loc 1 149 0
	cmpl	%ebx, %r12d
	.p2align 4,,2
	.p2align 3
	jg	.L55
	.loc 1 150 0
	movq	(%rbp), %rdi
	.p2align 4,,5
	.p2align 3
	call	usage
.LVL64:
.L55:
	.loc 1 151 0
	movslq	%ebx,%rax
.LVL65:
	movq	(%rbp,%rax,8), %rdi
	call	atoi
.LVL66:
	movl	%eax, 8(%rsp)
.LVL67:
	.loc 1 152 0
	testl	%eax, %eax
	jns	.L42
	.loc 1 153 0
	movq	(%rbp), %rdi
	call	usage
	jmp	.L42
.LVL68:
.L46:
	.loc 1 156 0
	addl	$1, %ebx
.LVL69:
	.loc 1 157 0
	cmpl	%ebx, %r12d
	.p2align 4,,2
	.p2align 3
	jg	.L56
	.loc 1 158 0
	movq	(%rbp), %rdi
	.p2align 4,,5
	.p2align 3
	call	usage
.LVL70:
.L56:
	.loc 1 159 0
	movslq	%ebx,%rax
.LVL71:
	movq	(%rbp,%rax,8), %rdi
	call	atoi
.LVL72:
	movl	%eax, 12(%rsp)
.LVL73:
	jmp	.L42
.LVL74:
.L45:
	.loc 1 162 0
	addl	$1, %ebx
.LVL75:
	.loc 1 163 0
	cmpl	%ebx, %r12d
	jg	.L57
	.loc 1 164 0
	movq	(%rbp), %rdi
	call	usage
.LVL76:
.L57:
	.loc 1 165 0
	movslq	%ebx,%rax
.LVL77:
	movq	(%rbp,%rax,8), %rdi
	call	atof
.LVL78:
	movsd	%xmm0, (%rsp)
.LVL79:
	.loc 1 166 0
	xorpd	%xmm2, %xmm2
	ucomisd	%xmm0, %xmm2
	ja	.L58
	ucomisd	.LC13(%rip), %xmm0
	jbe	.L42
.L58:
	.loc 1 167 0
	movq	(%rbp), %rdi
	call	usage
	jmp	.L42
.LVL80:
.L51:
	.loc 1 170 0
	addl	$1, %ebx
.LVL81:
	.loc 1 171 0
	cmpl	%ebx, %r12d
	.p2align 4,,2
	.p2align 3
	jg	.L60
	.loc 1 172 0
	movq	(%rbp), %rdi
	.p2align 4,,5
	.p2align 3
	call	usage
.LVL82:
.L60:
	.loc 1 173 0
	movslq	%ebx,%rax
.LVL83:
	movq	(%rbp,%rax,8), %rdi
	call	atoi
.LVL84:
	movl	%eax, %r15d
	.loc 1 174 0
	testl	%eax, %eax
	jg	.L42
.LVL85:
	.loc 1 175 0
	movq	(%rbp), %rdi
	call	usage
.LVL86:
	.p2align 4,,2
	.p2align 3
	jmp	.L42
.LVL87:
.L43:
	.loc 1 178 0
	movq	(%rbp), %rdi
	call	usage
.LVL88:
.L42:
	.loc 1 124 0
	addl	$1, %ebx
.LVL89:
	cmpl	%ebx, %r12d
	.p2align 4,,2
	.p2align 3
	jg	.L61
.LVL90:
	.loc 1 182 0
	movl	%r13d, %edi
	call	setup
	.loc 1 183 0
	cmpq	$tpsum1, %r14
	je	.L62
	.loc 1 184 0
	movl	%r13d, %esi
	movq	%r14, %rdi
	call	check
.L62:
	.loc 1 185 0
	cmpl	$1, %r15d
	je	.L63
.L68:
.LBB19:
.LBB20:
.LBB21:
	.loc 2 105 0
	movl	8(%rsp), %ebx
.LVL91:
.LBE21:
.LBE20:
.LBE19:
.LBB24:
	.loc 1 191 0
	testl	%ebx, %ebx
	jne	.L70
	.p2align 4,,4
	.p2align 3
	jmp	.L65
.LVL92:
.L63:
.LBE24:
.LBB29:
	.loc 1 186 0
	movl	%r13d, %esi
	movq	%r14, %rdi
	call	measure_function
.LBB23:
.LBB22:
	.loc 2 105 0
	cvtsi2sd	%r13d, %xmm1
	movapd	%xmm0, %xmm2
.LVL93:
	divsd	%xmm1, %xmm2
	movapd	%xmm2, %xmm1
	movl	$.LC14, %esi
	movl	$1, %edi
	movl	$2, %eax
	call	__printf_chk
.LVL94:
	jmp	.L66
.LVL95:
.L70:
.LBE22:
.LBE23:
.LBE29:
.LBB30:
	.loc 1 192 0
	movl	12(%rsp), %edi
	call	srandom
	.loc 1 193 0
	movl	16(%rsp), %r9d
	movsd	(%rsp), %xmm0
	movl	20(%rsp), %r8d
	movq	stdout(%rip), %rcx
	movl	%r15d, %edx
	movl	%r13d, %esi
	movq	%r14, %rdi
	call	find_cpe_full
.LBB25:
.LBB26:
	.loc 2 105 0
	movl	$.LC15, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE26:
.LBE25:
	.loc 1 191 0
	subl	$1, %ebx
	jne	.L70
.L65:
	.loc 1 196 0
	movl	%r13d, %esi
	movq	%r14, %rdi
	call	find_cpe
.LBB27:
.LBB28:
	.loc 2 105 0
	movl	$.LC16, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LVL96:
.L66:
.LBE28:
.LBE27:
.LBE30:
	.loc 1 200 0
	movl	$0, %eax
	addq	$24, %rsp
	popq	%rbx
.LVL97:
	popq	%rbp
.LVL98:
	popq	%r12
.LVL99:
	popq	%r13
.LVL100:
	popq	%r14
.LVL101:
	popq	%r15
.LVL102:
	ret
.LVL103:
.L41:
	.loc 1 182 0
	movl	$1024, %edi
.LVL104:
	call	setup
	movl	$tpsum1, %r14d
.LVL105:
	movl	$1024, %r13d
.LVL106:
	movl	$8, %r15d
.LVL107:
	xorpd	%xmm0, %xmm0
	movsd	%xmm0, (%rsp)
.LVL108:
	movl	$1, 8(%rsp)
.LVL109:
	movl	$0, 20(%rsp)
.LVL110:
	movl	$2, 16(%rsp)
.LVL111:
	movl	$31415, 12(%rsp)
.LVL112:
	jmp	.L68
.LFE28:
	.size	main, .-main
	.local	data
	.comm	data,3145728,32
	.local	a
	.comm	a,8,8
	.local	b
	.comm	b,8,8
	.local	c
	.comm	c,8,8
	.local	p
	.comm	p,8,8
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC13:
	.long	0
	.long	1072693248
	.section	.debug_frame,"",@progbits
.Lframe0:
	.long	.LECIE0-.LSCIE0
.LSCIE0:
	.long	0xffffffff
	.byte	0x1
	.string	""
	.uleb128 0x1
	.sleb128 -8
	.byte	0x10
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.byte	0x90
	.uleb128 0x1
	.align 8
.LECIE0:
.LSFDE0:
	.long	.LEFDE0-.LASFDE0
.LASFDE0:
	.long	.Lframe0
	.quad	.LFB19
	.quad	.LFE19-.LFB19
	.align 8
.LEFDE0:
.LSFDE2:
	.long	.LEFDE2-.LASFDE2
.LASFDE2:
	.long	.Lframe0
	.quad	.LFB20
	.quad	.LFE20-.LFB20
	.align 8
.LEFDE2:
.LSFDE4:
	.long	.LEFDE4-.LASFDE4
.LASFDE4:
	.long	.Lframe0
	.quad	.LFB21
	.quad	.LFE21-.LFB21
	.align 8
.LEFDE4:
.LSFDE6:
	.long	.LEFDE6-.LASFDE6
.LASFDE6:
	.long	.Lframe0
	.quad	.LFB22
	.quad	.LFE22-.LFB22
	.align 8
.LEFDE6:
.LSFDE8:
	.long	.LEFDE8-.LASFDE8
.LASFDE8:
	.long	.Lframe0
	.quad	.LFB23
	.quad	.LFE23-.LFB23
	.align 8
.LEFDE8:
.LSFDE10:
	.long	.LEFDE10-.LASFDE10
.LASFDE10:
	.long	.Lframe0
	.quad	.LFB24
	.quad	.LFE24-.LFB24
	.align 8
.LEFDE10:
.LSFDE12:
	.long	.LEFDE12-.LASFDE12
.LASFDE12:
	.long	.Lframe0
	.quad	.LFB25
	.quad	.LFE25-.LFB25
	.align 8
.LEFDE12:
.LSFDE14:
	.long	.LEFDE14-.LASFDE14
.LASFDE14:
	.long	.Lframe0
	.quad	.LFB27
	.quad	.LFE27-.LFB27
	.byte	0x4
	.long	.LCFI0-.LFB27
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE14:
.LSFDE16:
	.long	.LEFDE16-.LASFDE16
.LASFDE16:
	.long	.Lframe0
	.quad	.LFB26
	.quad	.LFE26-.LFB26
	.byte	0x4
	.long	.LCFI1-.LFB26
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0x40
	.byte	0x83
	.uleb128 0x7
	.byte	0x86
	.uleb128 0x6
	.byte	0x8c
	.uleb128 0x5
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8f
	.uleb128 0x2
	.align 8
.LEFDE16:
.LSFDE18:
	.long	.LEFDE18-.LASFDE18
.LASFDE18:
	.long	.Lframe0
	.quad	.LFB28
	.quad	.LFE28-.LFB28
	.byte	0x4
	.long	.LCFI8-.LFB28
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI9-.LCFI8
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI10-.LCFI9
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI11-.LCFI10
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI12-.LCFI11
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI13-.LCFI12
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI14-.LCFI13
	.byte	0xe
	.uleb128 0x50
	.byte	0x83
	.uleb128 0x7
	.byte	0x86
	.uleb128 0x6
	.byte	0x8c
	.uleb128 0x5
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8f
	.uleb128 0x2
	.align 8
.LEFDE18:
	.section	.eh_frame,"a",@progbits
.Lframe1:
	.long	.LECIE1-.LSCIE1
.LSCIE1:
	.long	0x0
	.byte	0x1
	.string	"zR"
	.uleb128 0x1
	.sleb128 -8
	.byte	0x10
	.uleb128 0x1
	.byte	0x3
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.byte	0x90
	.uleb128 0x1
	.align 8
.LECIE1:
.LSFDE1:
	.long	.LEFDE1-.LASFDE1
.LASFDE1:
	.long	.LASFDE1-.Lframe1
	.long	.LFB19
	.long	.LFE19-.LFB19
	.uleb128 0x0
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB20
	.long	.LFE20-.LFB20
	.uleb128 0x0
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB21
	.long	.LFE21-.LFB21
	.uleb128 0x0
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB22
	.long	.LFE22-.LFB22
	.uleb128 0x0
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB23
	.long	.LFE23-.LFB23
	.uleb128 0x0
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB24
	.long	.LFE24-.LFB24
	.uleb128 0x0
	.align 8
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.long	.LFB25
	.long	.LFE25-.LFB25
	.uleb128 0x0
	.align 8
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.long	.LFB27
	.long	.LFE27-.LFB27
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB27
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE15:
.LSFDE17:
	.long	.LEFDE17-.LASFDE17
.LASFDE17:
	.long	.LASFDE17-.Lframe1
	.long	.LFB26
	.long	.LFE26-.LFB26
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI1-.LFB26
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0x40
	.byte	0x83
	.uleb128 0x7
	.byte	0x86
	.uleb128 0x6
	.byte	0x8c
	.uleb128 0x5
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8f
	.uleb128 0x2
	.align 8
.LEFDE17:
.LSFDE19:
	.long	.LEFDE19-.LASFDE19
.LASFDE19:
	.long	.LASFDE19-.Lframe1
	.long	.LFB28
	.long	.LFE28-.LFB28
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI8-.LFB28
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI9-.LCFI8
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI10-.LCFI9
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI11-.LCFI10
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI12-.LCFI11
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI13-.LCFI12
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI14-.LCFI13
	.byte	0xe
	.uleb128 0x50
	.byte	0x83
	.uleb128 0x7
	.byte	0x86
	.uleb128 0x6
	.byte	0x8c
	.uleb128 0x5
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8f
	.uleb128 0x2
	.align 8
.LEFDE19:
	.text
.Letext0:
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST1:
	.quad	.LVL0-.Ltext0
	.quad	.LVL2-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST4:
	.quad	.LVL6-.Ltext0
	.quad	.LVL7-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL7-.Ltext0
	.quad	.LVL10-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	.LVL10-.Ltext0
	.quad	.LVL11-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL12-.Ltext0
	.quad	.LFE21-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	0x0
	.quad	0x0
.LLST5:
	.quad	.LVL8-.Ltext0
	.quad	.LVL9-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	0x0
	.quad	0x0
.LLST7:
	.quad	.LVL14-.Ltext0
	.quad	.LVL16-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL17-.Ltext0
	.quad	.LFE22-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	0x0
	.quad	0x0
.LLST9:
	.quad	.LVL18-.Ltext0
	.quad	.LVL19-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL19-.Ltext0
	.quad	.LVL20-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	0x0
	.quad	0x0
.LLST11:
	.quad	.LVL21-.Ltext0
	.quad	.LVL22-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL22-.Ltext0
	.quad	.LVL23-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	0x0
	.quad	0x0
.LLST13:
	.quad	.LVL24-.Ltext0
	.quad	.LVL25-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL25-.Ltext0
	.quad	.LVL26-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	0x0
	.quad	0x0
.LLST14:
	.quad	.LFB27-.Ltext0
	.quad	.LCFI0-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI0-.Ltext0
	.quad	.LFE27-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST15:
	.quad	.LVL27-.Ltext0
	.quad	.LVL28-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST16:
	.quad	.LFB26-.Ltext0
	.quad	.LCFI1-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI1-.Ltext0
	.quad	.LCFI2-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI2-.Ltext0
	.quad	.LCFI3-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 24
	.quad	.LCFI3-.Ltext0
	.quad	.LCFI4-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 32
	.quad	.LCFI4-.Ltext0
	.quad	.LCFI5-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 40
	.quad	.LCFI5-.Ltext0
	.quad	.LCFI6-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 48
	.quad	.LCFI6-.Ltext0
	.quad	.LCFI7-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 56
	.quad	.LCFI7-.Ltext0
	.quad	.LFE26-.Ltext0
	.value	0x3
	.byte	0x77
	.sleb128 64
	.quad	0x0
	.quad	0x0
.LLST17:
	.quad	.LVL29-.Ltext0
	.quad	.LVL30-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL30-.Ltext0
	.quad	.LVL41-.Ltext0
	.value	0x1
	.byte	0x5f
	.quad	0x0
	.quad	0x0
.LLST18:
	.quad	.LVL29-.Ltext0
	.quad	.LVL32-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL32-.Ltext0
	.quad	.LVL35-.Ltext0
	.value	0x1
	.byte	0x5d
	.quad	.LVL35-.Ltext0
	.quad	.LVL36-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL36-.Ltext0
	.quad	.LVL40-.Ltext0
	.value	0x1
	.byte	0x5d
	.quad	0x0
	.quad	0x0
.LLST19:
	.quad	.LVL31-.Ltext0
	.quad	.LVL33-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	.LVL33-.Ltext0
	.quad	.LVL35-.Ltext0
	.value	0x1
	.byte	0x56
	.quad	.LVL37-.Ltext0
	.quad	.LVL38-.Ltext0
	.value	0x1
	.byte	0x56
	.quad	0x0
	.quad	0x0
.LLST20:
	.quad	.LVL34-.Ltext0
	.quad	.LVL35-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	.LVL37-.Ltext0
	.quad	.LVL39-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	0x0
	.quad	0x0
.LLST21:
	.quad	.LFB28-.Ltext0
	.quad	.LCFI8-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI8-.Ltext0
	.quad	.LCFI9-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI9-.Ltext0
	.quad	.LCFI10-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 24
	.quad	.LCFI10-.Ltext0
	.quad	.LCFI11-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 32
	.quad	.LCFI11-.Ltext0
	.quad	.LCFI12-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 40
	.quad	.LCFI12-.Ltext0
	.quad	.LCFI13-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 48
	.quad	.LCFI13-.Ltext0
	.quad	.LCFI14-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 56
	.quad	.LCFI14-.Ltext0
	.quad	.LFE28-.Ltext0
	.value	0x3
	.byte	0x77
	.sleb128 80
	.quad	0x0
	.quad	0x0
.LLST22:
	.quad	.LVL42-.Ltext0
	.quad	.LVL53-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL53-.Ltext0
	.quad	.LVL99-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	.LVL103-.Ltext0
	.quad	.LVL104-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL104-.Ltext0
	.quad	.LFE28-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	0x0
	.quad	0x0
.LLST23:
	.quad	.LVL42-.Ltext0
	.quad	.LVL43-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL43-.Ltext0
	.quad	.LVL98-.Ltext0
	.value	0x1
	.byte	0x56
	.quad	.LVL103-.Ltext0
	.quad	.LFE28-.Ltext0
	.value	0x1
	.byte	0x56
	.quad	0x0
	.quad	0x0
.LLST24:
	.quad	.LVL44-.Ltext0
	.quad	.LVL101-.Ltext0
	.value	0x1
	.byte	0x5e
	.quad	.LVL105-.Ltext0
	.quad	.LFE28-.Ltext0
	.value	0x1
	.byte	0x5e
	.quad	0x0
	.quad	0x0
.LLST25:
	.quad	.LVL45-.Ltext0
	.quad	.LVL100-.Ltext0
	.value	0x1
	.byte	0x5d
	.quad	.LVL106-.Ltext0
	.quad	.LFE28-.Ltext0
	.value	0x1
	.byte	0x5d
	.quad	0x0
	.quad	0x0
.LLST26:
	.quad	.LVL46-.Ltext0
	.quad	.LVL54-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	.LVL54-.Ltext0
	.quad	.LVL55-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL58-.Ltext0
	.quad	.LVL60-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	.LVL60-.Ltext0
	.quad	.LVL61-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL63-.Ltext0
	.quad	.LVL65-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	.LVL65-.Ltext0
	.quad	.LVL66-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL69-.Ltext0
	.quad	.LVL71-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	.LVL71-.Ltext0
	.quad	.LVL72-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL75-.Ltext0
	.quad	.LVL77-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	.LVL77-.Ltext0
	.quad	.LVL78-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL81-.Ltext0
	.quad	.LVL83-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	.LVL83-.Ltext0
	.quad	.LVL84-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL89-.Ltext0
	.quad	.LVL91-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	.LVL92-.Ltext0
	.quad	.LVL95-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	.LVL96-.Ltext0
	.quad	.LVL97-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	0x0
	.quad	0x0
.LLST27:
	.quad	.LVL47-.Ltext0
	.quad	.LVL85-.Ltext0
	.value	0x1
	.byte	0x5f
	.quad	.LVL85-.Ltext0
	.quad	.LVL86-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL86-.Ltext0
	.quad	.LVL102-.Ltext0
	.value	0x1
	.byte	0x5f
	.quad	.LVL107-.Ltext0
	.quad	.LFE28-.Ltext0
	.value	0x1
	.byte	0x5f
	.quad	0x0
	.quad	0x0
.LLST28:
	.quad	.LVL48-.Ltext0
	.quad	.LVL103-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 0
	.quad	.LVL108-.Ltext0
	.quad	.LFE28-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 0
	.quad	0x0
	.quad	0x0
.LLST29:
	.quad	.LVL49-.Ltext0
	.quad	.LVL67-.Ltext0
	.value	0x3
	.byte	0x91
	.sleb128 -72
	.quad	.LVL67-.Ltext0
	.quad	.LVL68-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LVL68-.Ltext0
	.quad	.LVL88-.Ltext0
	.value	0x3
	.byte	0x91
	.sleb128 -72
	.quad	.LVL88-.Ltext0
	.quad	.LVL90-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LVL90-.Ltext0
	.quad	.LVL103-.Ltext0
	.value	0x3
	.byte	0x91
	.sleb128 -72
	.quad	.LVL109-.Ltext0
	.quad	.LFE28-.Ltext0
	.value	0x3
	.byte	0x91
	.sleb128 -72
	.quad	0x0
	.quad	0x0
.LLST30:
	.quad	.LVL50-.Ltext0
	.quad	.LVL73-.Ltext0
	.value	0x3
	.byte	0x91
	.sleb128 -68
	.quad	.LVL73-.Ltext0
	.quad	.LVL74-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 12
	.quad	.LVL74-.Ltext0
	.quad	.LVL88-.Ltext0
	.value	0x3
	.byte	0x91
	.sleb128 -68
	.quad	.LVL88-.Ltext0
	.quad	.LVL90-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 12
	.quad	.LVL90-.Ltext0
	.quad	.LVL103-.Ltext0
	.value	0x3
	.byte	0x91
	.sleb128 -68
	.quad	.LVL112-.Ltext0
	.quad	.LFE28-.Ltext0
	.value	0x3
	.byte	0x91
	.sleb128 -68
	.quad	0x0
	.quad	0x0
.LLST31:
	.quad	.LVL51-.Ltext0
	.quad	.LVL103-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 -64
	.quad	.LVL111-.Ltext0
	.quad	.LFE28-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 -64
	.quad	0x0
	.quad	0x0
.LLST32:
	.quad	.LVL52-.Ltext0
	.quad	.LVL103-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 -60
	.quad	.LVL110-.Ltext0
	.quad	.LFE28-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 -60
	.quad	0x0
	.quad	0x0
.LLST33:
	.quad	.LVL93-.Ltext0
	.quad	.LVL94-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	0x0
	.quad	0x0
	.file 3 "/usr/lib/gcc/x86_64-linux-gnu/4.3.2/include/stddef.h"
	.file 4 "/usr/include/bits/types.h"
	.file 5 "/usr/include/libio.h"
	.file 6 "/usr/include/stdio.h"
	.file 7 "../include/cpe.h"
	.section	.debug_info
	.long	0x7d5
	.value	0x2
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF78
	.byte	0x1
	.long	.LASF79
	.long	.LASF80
	.quad	.Ltext0
	.quad	.Letext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.long	.LASF7
	.byte	0x3
	.byte	0xd6
	.long	0x38
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.long	.LASF0
	.uleb128 0x3
	.byte	0x1
	.byte	0x8
	.long	.LASF1
	.uleb128 0x3
	.byte	0x2
	.byte	0x7
	.long	.LASF2
	.uleb128 0x3
	.byte	0x4
	.byte	0x7
	.long	.LASF3
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.long	.LASF4
	.uleb128 0x3
	.byte	0x2
	.byte	0x5
	.long	.LASF5
	.uleb128 0x4
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.long	.LASF6
	.uleb128 0x2
	.long	.LASF8
	.byte	0x4
	.byte	0x8d
	.long	0x69
	.uleb128 0x2
	.long	.LASF9
	.byte	0x4
	.byte	0x8e
	.long	0x69
	.uleb128 0x5
	.byte	0x8
	.byte	0x7
	.uleb128 0x6
	.byte	0x8
	.uleb128 0x7
	.byte	0x8
	.long	0x91
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.long	.LASF10
	.uleb128 0x8
	.long	.LASF40
	.byte	0xd8
	.byte	0x6
	.byte	0x2d
	.long	0x264
	.uleb128 0x9
	.long	.LASF11
	.byte	0x5
	.value	0x110
	.long	0x62
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0x9
	.long	.LASF12
	.byte	0x5
	.value	0x115
	.long	0x8b
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x9
	.long	.LASF13
	.byte	0x5
	.value	0x116
	.long	0x8b
	.byte	0x2
	.byte	0x23
	.uleb128 0x10
	.uleb128 0x9
	.long	.LASF14
	.byte	0x5
	.value	0x117
	.long	0x8b
	.byte	0x2
	.byte	0x23
	.uleb128 0x18
	.uleb128 0x9
	.long	.LASF15
	.byte	0x5
	.value	0x118
	.long	0x8b
	.byte	0x2
	.byte	0x23
	.uleb128 0x20
	.uleb128 0x9
	.long	.LASF16
	.byte	0x5
	.value	0x119
	.long	0x8b
	.byte	0x2
	.byte	0x23
	.uleb128 0x28
	.uleb128 0x9
	.long	.LASF17
	.byte	0x5
	.value	0x11a
	.long	0x8b
	.byte	0x2
	.byte	0x23
	.uleb128 0x30
	.uleb128 0x9
	.long	.LASF18
	.byte	0x5
	.value	0x11b
	.long	0x8b
	.byte	0x2
	.byte	0x23
	.uleb128 0x38
	.uleb128 0x9
	.long	.LASF19
	.byte	0x5
	.value	0x11c
	.long	0x8b
	.byte	0x2
	.byte	0x23
	.uleb128 0x40
	.uleb128 0x9
	.long	.LASF20
	.byte	0x5
	.value	0x11e
	.long	0x8b
	.byte	0x2
	.byte	0x23
	.uleb128 0x48
	.uleb128 0x9
	.long	.LASF21
	.byte	0x5
	.value	0x11f
	.long	0x8b
	.byte	0x2
	.byte	0x23
	.uleb128 0x50
	.uleb128 0x9
	.long	.LASF22
	.byte	0x5
	.value	0x120
	.long	0x8b
	.byte	0x2
	.byte	0x23
	.uleb128 0x58
	.uleb128 0x9
	.long	.LASF23
	.byte	0x5
	.value	0x122
	.long	0x2a2
	.byte	0x2
	.byte	0x23
	.uleb128 0x60
	.uleb128 0x9
	.long	.LASF24
	.byte	0x5
	.value	0x124
	.long	0x2a8
	.byte	0x2
	.byte	0x23
	.uleb128 0x68
	.uleb128 0x9
	.long	.LASF25
	.byte	0x5
	.value	0x126
	.long	0x62
	.byte	0x2
	.byte	0x23
	.uleb128 0x70
	.uleb128 0x9
	.long	.LASF26
	.byte	0x5
	.value	0x12a
	.long	0x62
	.byte	0x2
	.byte	0x23
	.uleb128 0x74
	.uleb128 0x9
	.long	.LASF27
	.byte	0x5
	.value	0x12c
	.long	0x70
	.byte	0x2
	.byte	0x23
	.uleb128 0x78
	.uleb128 0x9
	.long	.LASF28
	.byte	0x5
	.value	0x130
	.long	0x46
	.byte	0x3
	.byte	0x23
	.uleb128 0x80
	.uleb128 0x9
	.long	.LASF29
	.byte	0x5
	.value	0x131
	.long	0x54
	.byte	0x3
	.byte	0x23
	.uleb128 0x82
	.uleb128 0x9
	.long	.LASF30
	.byte	0x5
	.value	0x132
	.long	0x2ae
	.byte	0x3
	.byte	0x23
	.uleb128 0x83
	.uleb128 0x9
	.long	.LASF31
	.byte	0x5
	.value	0x136
	.long	0x2be
	.byte	0x3
	.byte	0x23
	.uleb128 0x88
	.uleb128 0x9
	.long	.LASF32
	.byte	0x5
	.value	0x13f
	.long	0x7b
	.byte	0x3
	.byte	0x23
	.uleb128 0x90
	.uleb128 0x9
	.long	.LASF33
	.byte	0x5
	.value	0x148
	.long	0x89
	.byte	0x3
	.byte	0x23
	.uleb128 0x98
	.uleb128 0x9
	.long	.LASF34
	.byte	0x5
	.value	0x149
	.long	0x89
	.byte	0x3
	.byte	0x23
	.uleb128 0xa0
	.uleb128 0x9
	.long	.LASF35
	.byte	0x5
	.value	0x14a
	.long	0x89
	.byte	0x3
	.byte	0x23
	.uleb128 0xa8
	.uleb128 0x9
	.long	.LASF36
	.byte	0x5
	.value	0x14b
	.long	0x89
	.byte	0x3
	.byte	0x23
	.uleb128 0xb0
	.uleb128 0x9
	.long	.LASF37
	.byte	0x5
	.value	0x14c
	.long	0x2d
	.byte	0x3
	.byte	0x23
	.uleb128 0xb8
	.uleb128 0x9
	.long	.LASF38
	.byte	0x5
	.value	0x14e
	.long	0x62
	.byte	0x3
	.byte	0x23
	.uleb128 0xc0
	.uleb128 0x9
	.long	.LASF39
	.byte	0x5
	.value	0x150
	.long	0x2c4
	.byte	0x3
	.byte	0x23
	.uleb128 0xc4
	.byte	0x0
	.uleb128 0xa
	.long	.LASF81
	.byte	0x5
	.byte	0xb4
	.uleb128 0x8
	.long	.LASF41
	.byte	0x18
	.byte	0x5
	.byte	0xba
	.long	0x2a2
	.uleb128 0xb
	.long	.LASF42
	.byte	0x5
	.byte	0xbb
	.long	0x2a2
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0xb
	.long	.LASF43
	.byte	0x5
	.byte	0xbc
	.long	0x2a8
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0xb
	.long	.LASF44
	.byte	0x5
	.byte	0xc0
	.long	0x62
	.byte	0x2
	.byte	0x23
	.uleb128 0x10
	.byte	0x0
	.uleb128 0x7
	.byte	0x8
	.long	0x26b
	.uleb128 0x7
	.byte	0x8
	.long	0x98
	.uleb128 0xc
	.long	0x91
	.long	0x2be
	.uleb128 0xd
	.long	0x86
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.byte	0x8
	.long	0x264
	.uleb128 0xc
	.long	0x91
	.long	0x2d4
	.uleb128 0xd
	.long	0x86
	.byte	0x13
	.byte	0x0
	.uleb128 0x7
	.byte	0x8
	.long	0x2da
	.uleb128 0xe
	.long	0x91
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.long	.LASF45
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.long	.LASF46
	.uleb128 0x2
	.long	.LASF47
	.byte	0x7
	.byte	0x4
	.long	0x2f8
	.uleb128 0x7
	.byte	0x8
	.long	0x2fe
	.uleb128 0xf
	.byte	0x1
	.long	0x30a
	.uleb128 0x10
	.long	0x62
	.byte	0x0
	.uleb128 0x11
	.byte	0x4
	.byte	0x7
	.byte	0xb
	.long	0x31f
	.uleb128 0x12
	.long	.LASF48
	.sleb128 0
	.uleb128 0x12
	.long	.LASF49
	.sleb128 1
	.byte	0x0
	.uleb128 0x2
	.long	.LASF50
	.byte	0x7
	.byte	0xc
	.long	0x30a
	.uleb128 0x13
	.byte	0x1
	.long	.LASF82
	.byte	0x2
	.byte	0x68
	.byte	0x1
	.long	0x62
	.byte	0x2
	.long	0x349
	.uleb128 0x14
	.long	.LASF83
	.byte	0x2
	.byte	0x67
	.long	0x2d4
	.uleb128 0x15
	.byte	0x0
	.uleb128 0x16
	.long	.LASF60
	.byte	0x1
	.byte	0xc
	.byte	0x1
	.quad	.LFB19
	.quad	.LFE19
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x378
	.uleb128 0x17
	.string	"len"
	.byte	0x1
	.byte	0xb
	.long	0x62
	.long	.LLST1
	.byte	0x0
	.uleb128 0x18
	.byte	0x1
	.long	.LASF52
	.byte	0x1
	.byte	0x16
	.byte	0x1
	.quad	.LFB20
	.quad	.LFE20
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x3c5
	.uleb128 0x19
	.string	"a"
	.byte	0x1
	.byte	0x15
	.long	0x3c5
	.byte	0x1
	.byte	0x55
	.uleb128 0x19
	.string	"p"
	.byte	0x1
	.byte	0x15
	.long	0x3c5
	.byte	0x1
	.byte	0x54
	.uleb128 0x19
	.string	"n"
	.byte	0x1
	.byte	0x15
	.long	0x69
	.byte	0x1
	.byte	0x51
	.uleb128 0x1a
	.string	"i"
	.byte	0x1
	.byte	0x17
	.long	0x69
	.byte	0x1
	.byte	0x50
	.byte	0x0
	.uleb128 0x7
	.byte	0x8
	.long	0x3cb
	.uleb128 0x3
	.byte	0x4
	.byte	0x4
	.long	.LASF51
	.uleb128 0x18
	.byte	0x1
	.long	.LASF53
	.byte	0x1
	.byte	0x1e
	.byte	0x1
	.quad	.LFB21
	.quad	.LFE21
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x442
	.uleb128 0x19
	.string	"a"
	.byte	0x1
	.byte	0x1d
	.long	0x3c5
	.byte	0x1
	.byte	0x55
	.uleb128 0x19
	.string	"p"
	.byte	0x1
	.byte	0x1d
	.long	0x3c5
	.byte	0x1
	.byte	0x54
	.uleb128 0x19
	.string	"n"
	.byte	0x1
	.byte	0x1d
	.long	0x69
	.byte	0x1
	.byte	0x51
	.uleb128 0x1b
	.string	"i"
	.byte	0x1
	.byte	0x1f
	.long	0x69
	.long	.LLST4
	.uleb128 0x1c
	.quad	.LBB14
	.quad	.LBE14
	.uleb128 0x1d
	.long	.LASF54
	.byte	0x1
	.byte	0x22
	.long	0x3cb
	.long	.LLST5
	.byte	0x0
	.byte	0x0
	.uleb128 0x18
	.byte	0x1
	.long	.LASF55
	.byte	0x1
	.byte	0x2e
	.byte	0x1
	.quad	.LFB22
	.quad	.LFE22
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x4ab
	.uleb128 0x19
	.string	"a"
	.byte	0x1
	.byte	0x2d
	.long	0x3c5
	.byte	0x1
	.byte	0x55
	.uleb128 0x19
	.string	"p"
	.byte	0x1
	.byte	0x2d
	.long	0x3c5
	.byte	0x1
	.byte	0x54
	.uleb128 0x19
	.string	"n"
	.byte	0x1
	.byte	0x2d
	.long	0x69
	.byte	0x1
	.byte	0x51
	.uleb128 0x1a
	.string	"i"
	.byte	0x1
	.byte	0x2f
	.long	0x69
	.byte	0x1
	.byte	0x50
	.uleb128 0x1d
	.long	.LASF56
	.byte	0x1
	.byte	0x31
	.long	0x3cb
	.long	.LLST7
	.uleb128 0x1a
	.string	"val"
	.byte	0x1
	.byte	0x31
	.long	0x3cb
	.byte	0x1
	.byte	0x61
	.byte	0x0
	.uleb128 0x18
	.byte	0x1
	.long	.LASF57
	.byte	0x1
	.byte	0x3c
	.byte	0x1
	.quad	.LFB23
	.quad	.LFE23
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x4db
	.uleb128 0x17
	.string	"cnt"
	.byte	0x1
	.byte	0x3b
	.long	0x62
	.long	.LLST9
	.byte	0x0
	.uleb128 0x18
	.byte	0x1
	.long	.LASF58
	.byte	0x1
	.byte	0x41
	.byte	0x1
	.quad	.LFB24
	.quad	.LFE24
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x50b
	.uleb128 0x17
	.string	"cnt"
	.byte	0x1
	.byte	0x40
	.long	0x62
	.long	.LLST11
	.byte	0x0
	.uleb128 0x18
	.byte	0x1
	.long	.LASF59
	.byte	0x1
	.byte	0x46
	.byte	0x1
	.quad	.LFB25
	.quad	.LFE25
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x53b
	.uleb128 0x17
	.string	"cnt"
	.byte	0x1
	.byte	0x45
	.long	0x62
	.long	.LLST13
	.byte	0x0
	.uleb128 0x1e
	.long	.LASF61
	.byte	0x1
	.byte	0x63
	.byte	0x1
	.quad	.LFB27
	.quad	.LFE27
	.long	.LLST14
	.long	0x588
	.uleb128 0x1f
	.long	.LASF62
	.byte	0x1
	.byte	0x62
	.long	0x8b
	.long	.LLST15
	.uleb128 0x20
	.long	0x32a
	.quad	.LBB15
	.quad	.LBE15
	.byte	0x1
	.byte	0x64
	.uleb128 0x21
	.long	0x33c
	.byte	0x0
	.byte	0x0
	.uleb128 0x22
	.byte	0x1
	.long	.LASF63
	.byte	0x1
	.byte	0x4b
	.byte	0x1
	.quad	.LFB26
	.quad	.LFE26
	.long	.LLST16
	.long	0x601
	.uleb128 0x1f
	.long	.LASF64
	.byte	0x1
	.byte	0x4b
	.long	0x2ed
	.long	.LLST17
	.uleb128 0x17
	.string	"cnt"
	.byte	0x1
	.byte	0x4b
	.long	0x62
	.long	.LLST18
	.uleb128 0x1b
	.string	"i"
	.byte	0x1
	.byte	0x4c
	.long	0x62
	.long	.LLST19
	.uleb128 0x1d
	.long	.LASF65
	.byte	0x1
	.byte	0x4d
	.long	0x62
	.long	.LLST20
	.uleb128 0x20
	.long	0x32a
	.quad	.LBB17
	.quad	.LBE17
	.byte	0x1
	.byte	0x57
	.uleb128 0x21
	.long	0x33c
	.byte	0x0
	.byte	0x0
	.uleb128 0x23
	.byte	0x1
	.long	.LASF84
	.byte	0x1
	.byte	0x72
	.byte	0x1
	.long	0x62
	.quad	.LFB28
	.quad	.LFE28
	.long	.LLST21
	.long	0x74a
	.uleb128 0x1f
	.long	.LASF66
	.byte	0x1
	.byte	0x71
	.long	0x62
	.long	.LLST22
	.uleb128 0x1f
	.long	.LASF67
	.byte	0x1
	.byte	0x71
	.long	0x74a
	.long	.LLST23
	.uleb128 0x1b
	.string	"f"
	.byte	0x1
	.byte	0x73
	.long	0x2ed
	.long	.LLST24
	.uleb128 0x1b
	.string	"cnt"
	.byte	0x1
	.byte	0x74
	.long	0x62
	.long	.LLST25
	.uleb128 0x1b
	.string	"i"
	.byte	0x1
	.byte	0x75
	.long	0x62
	.long	.LLST26
	.uleb128 0x1d
	.long	.LASF68
	.byte	0x1
	.byte	0x76
	.long	0x62
	.long	.LLST27
	.uleb128 0x1d
	.long	.LASF69
	.byte	0x1
	.byte	0x77
	.long	0x750
	.long	.LLST28
	.uleb128 0x1d
	.long	.LASF70
	.byte	0x1
	.byte	0x78
	.long	0x62
	.long	.LLST29
	.uleb128 0x1d
	.long	.LASF71
	.byte	0x1
	.byte	0x79
	.long	0x62
	.long	.LLST30
	.uleb128 0x1d
	.long	.LASF72
	.byte	0x1
	.byte	0x7a
	.long	0x62
	.long	.LLST31
	.uleb128 0x1d
	.long	.LASF73
	.byte	0x1
	.byte	0x7b
	.long	0x31f
	.long	.LLST32
	.uleb128 0x24
	.long	.Ldebug_ranges0+0x0
	.long	0x6ef
	.uleb128 0x1b
	.string	"t"
	.byte	0x1
	.byte	0xba
	.long	0x750
	.long	.LLST33
	.uleb128 0x25
	.long	0x32a
	.long	.Ldebug_ranges0+0x30
	.byte	0x1
	.byte	0xbb
	.uleb128 0x21
	.long	0x33c
	.byte	0x0
	.byte	0x0
	.uleb128 0x26
	.long	.Ldebug_ranges0+0x60
	.uleb128 0x27
	.long	.LASF74
	.byte	0x1
	.byte	0xbe
	.long	0x750
	.uleb128 0x27
	.long	.LASF75
	.byte	0x1
	.byte	0xbe
	.long	0x750
	.uleb128 0x28
	.long	0x32a
	.quad	.LBB25
	.quad	.LBE25
	.byte	0x1
	.byte	0xc2
	.long	0x72b
	.uleb128 0x21
	.long	0x33c
	.byte	0x0
	.uleb128 0x20
	.long	0x32a
	.quad	.LBB27
	.quad	.LBE27
	.byte	0x1
	.byte	0xc5
	.uleb128 0x21
	.long	0x33c
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.byte	0x8
	.long	0x8b
	.uleb128 0x3
	.byte	0x8
	.byte	0x4
	.long	.LASF76
	.uleb128 0xc
	.long	0x3cb
	.long	0x76a
	.uleb128 0x29
	.long	0x86
	.long	0xbffff
	.byte	0x0
	.uleb128 0x2a
	.long	.LASF77
	.byte	0x1
	.byte	0x8
	.long	0x757
	.byte	0x9
	.byte	0x3
	.quad	data
	.uleb128 0x1a
	.string	"a"
	.byte	0x1
	.byte	0x9
	.long	0x3c5
	.byte	0x9
	.byte	0x3
	.quad	a
	.uleb128 0x1a
	.string	"b"
	.byte	0x1
	.byte	0x9
	.long	0x3c5
	.byte	0x9
	.byte	0x3
	.quad	b
	.uleb128 0x1a
	.string	"c"
	.byte	0x1
	.byte	0x9
	.long	0x3c5
	.byte	0x9
	.byte	0x3
	.quad	c
	.uleb128 0x1a
	.string	"p"
	.byte	0x1
	.byte	0x9
	.long	0x3c5
	.byte	0x9
	.byte	0x3
	.quad	p
	.uleb128 0x2b
	.long	.LASF85
	.byte	0x6
	.byte	0x92
	.long	0x2a8
	.byte	0x1
	.byte	0x1
	.byte	0x0
	.section	.debug_abbrev
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x2
	.uleb128 0x16
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0x0
	.byte	0x0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0x0
	.byte	0x0
	.uleb128 0x5
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x6
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x9
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0xa
	.uleb128 0x16
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0xc
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xd
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0xe
	.uleb128 0x26
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xf
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x10
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x11
	.uleb128 0x4
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x12
	.uleb128 0x28
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0xd
	.byte	0x0
	.byte	0x0
	.uleb128 0x13
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x14
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x15
	.uleb128 0x18
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0x16
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x17
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x18
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x19
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x1a
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x1b
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x1d
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x1e
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1f
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x20
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x21
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x22
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x23
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x24
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x25
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x55
	.uleb128 0x6
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x26
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x27
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x28
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x29
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x2a
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x2b
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.section	.debug_pubnames,"",@progbits
	.long	0x62
	.value	0x2
	.long	.Ldebug_info0
	.long	0x7d9
	.long	0x378
	.string	"psum1"
	.long	0x3d2
	.string	"psum2"
	.long	0x442
	.string	"psum1a"
	.long	0x4ab
	.string	"tpsum1"
	.long	0x4db
	.string	"tpsum2"
	.long	0x50b
	.string	"tpsum1a"
	.long	0x588
	.string	"check"
	.long	0x601
	.string	"main"
	.long	0x0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0x0
	.value	0x0
	.value	0x0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0x0
	.quad	0x0
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.quad	.LBB19-.Ltext0
	.quad	.LBE19-.Ltext0
	.quad	.LBB29-.Ltext0
	.quad	.LBE29-.Ltext0
	.quad	0x0
	.quad	0x0
	.quad	.LBB20-.Ltext0
	.quad	.LBE20-.Ltext0
	.quad	.LBB23-.Ltext0
	.quad	.LBE23-.Ltext0
	.quad	0x0
	.quad	0x0
	.quad	.LBB24-.Ltext0
	.quad	.LBE24-.Ltext0
	.quad	.LBB30-.Ltext0
	.quad	.LBE30-.Ltext0
	.quad	0x0
	.quad	0x0
	.section	.debug_str,"MS",@progbits,1
.LASF82:
	.string	"printf"
.LASF8:
	.string	"__off_t"
.LASF12:
	.string	"_IO_read_ptr"
.LASF24:
	.string	"_chain"
.LASF7:
	.string	"size_t"
.LASF30:
	.string	"_shortbuf"
.LASF56:
	.string	"last_val"
.LASF18:
	.string	"_IO_buf_base"
.LASF45:
	.string	"long long unsigned int"
.LASF47:
	.string	"elem_fun_t"
.LASF57:
	.string	"tpsum1"
.LASF46:
	.string	"long long int"
.LASF4:
	.string	"signed char"
.LASF25:
	.string	"_fileno"
.LASF13:
	.string	"_IO_read_end"
.LASF6:
	.string	"long int"
.LASF63:
	.string	"check"
.LASF59:
	.string	"tpsum1a"
.LASF11:
	.string	"_flags"
.LASF19:
	.string	"_IO_buf_end"
.LASF28:
	.string	"_cur_column"
.LASF76:
	.string	"double"
.LASF49:
	.string	"RAN_SAMPLE"
.LASF27:
	.string	"_old_offset"
.LASF32:
	.string	"_offset"
.LASF75:
	.string	"cpe_def"
.LASF78:
	.string	"GNU C 4.3.2"
.LASF41:
	.string	"_IO_marker"
.LASF69:
	.string	"bias"
.LASF3:
	.string	"unsigned int"
.LASF68:
	.string	"samples"
.LASF71:
	.string	"seed"
.LASF0:
	.string	"long unsigned int"
.LASF16:
	.string	"_IO_write_ptr"
.LASF62:
	.string	"name"
.LASF43:
	.string	"_sbuf"
.LASF77:
	.string	"data"
.LASF2:
	.string	"short unsigned int"
.LASF73:
	.string	"smethod"
.LASF48:
	.string	"UNI_SAMPLE"
.LASF20:
	.string	"_IO_save_base"
.LASF31:
	.string	"_lock"
.LASF26:
	.string	"_flags2"
.LASF38:
	.string	"_mode"
.LASF85:
	.string	"stdout"
.LASF52:
	.string	"psum1"
.LASF53:
	.string	"psum2"
.LASF61:
	.string	"usage"
.LASF50:
	.string	"sample_t"
.LASF58:
	.string	"tpsum2"
.LASF17:
	.string	"_IO_write_end"
.LASF81:
	.string	"_IO_lock_t"
.LASF60:
	.string	"setup"
.LASF40:
	.string	"_IO_FILE"
.LASF51:
	.string	"float"
.LASF44:
	.string	"_pos"
.LASF23:
	.string	"_markers"
.LASF1:
	.string	"unsigned char"
.LASF64:
	.string	"tpsumX"
.LASF55:
	.string	"psum1a"
.LASF5:
	.string	"short int"
.LASF29:
	.string	"_vtable_offset"
.LASF10:
	.string	"char"
.LASF79:
	.string	"test_cpe.c"
.LASF54:
	.string	"mid_val"
.LASF42:
	.string	"_next"
.LASF9:
	.string	"__off64_t"
.LASF14:
	.string	"_IO_read_base"
.LASF22:
	.string	"_IO_save_end"
.LASF83:
	.string	"__fmt"
.LASF72:
	.string	"verbose"
.LASF33:
	.string	"__pad1"
.LASF34:
	.string	"__pad2"
.LASF35:
	.string	"__pad3"
.LASF36:
	.string	"__pad4"
.LASF37:
	.string	"__pad5"
.LASF39:
	.string	"_unused2"
.LASF67:
	.string	"argv"
.LASF21:
	.string	"_IO_backup_base"
.LASF70:
	.string	"reps"
.LASF66:
	.string	"argc"
.LASF74:
	.string	"cpe_full"
.LASF84:
	.string	"main"
.LASF15:
	.string	"_IO_write_base"
.LASF65:
	.string	"errcnt"
.LASF80:
	.string	"/home/rbryant/ics2/code/opt"
	.ident	"GCC: (Ubuntu 4.3.2-1ubuntu11) 4.3.2"
	.section	.note.GNU-stack,"",@progbits
