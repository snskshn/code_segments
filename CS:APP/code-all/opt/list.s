	.file	"list.c"
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.text
.Ltext0:
.globl mylink
	.type	mylink, @function
mylink:
.LFB14:
	.file 1 "list.c"
	.loc 1 18 0
.LVL0:
	.loc 1 21 0
	leal	-1(%rdi), %ecx
	testl	%ecx, %ecx
	jle	.L2
	movl	$main_list+16, %eax
	movl	$0, %edx
.LVL1:
.L3:
	.loc 1 22 0
	movl	%edx, -8(%rax)
	.loc 1 23 0
	movq	%rax, -16(%rax)
	.loc 1 21 0
	addl	$1, %edx
	addq	$16, %rax
	cmpl	%ecx, %edx
	jl	.L3
.L2:
	.loc 1 25 0
	movslq	%ecx,%rax
	movl	$main_list, %edx
.LVL2:
	salq	$4, %rax
	movl	%ecx, 8(%rax,%rdx)
	.loc 1 26 0
	movq	$0, (%rax,%rdx)
	.loc 1 27 0
	ret
.LFE14:
	.size	mylink, .-mylink
.globl list_len
	.type	list_len, @function
list_len:
.LFB15:
	.loc 1 30 0
.LVL3:
	.loc 1 32 0
	movl	$0, %eax
.LVL4:
	testq	%rdi, %rdi
	je	.L8
.L11:
	.loc 1 33 0
	addl	$1, %eax
	.loc 1 34 0
	movq	(%rdi), %rdi
.LVL5:
	.loc 1 32 0
	testq	%rdi, %rdi
	jne	.L11
.L8:
.LVL6:
	.loc 1 37 0
	rep
	ret
.LFE15:
	.size	list_len, .-list_len
.globl list_len_clear
	.type	list_len_clear, @function
list_len_clear:
.LFB16:
	.loc 1 41 0
.LVL7:
	.loc 1 43 0
	movl	$0, %eax
.LVL8:
	testq	%rdi, %rdi
	je	.L15
.L18:
	.loc 1 44 0
	movl	$0, 8(%rdi)
	.loc 1 45 0
	movq	(%rdi), %rdi
.LVL9:
	.loc 1 46 0
	addl	$1, %eax
	.loc 1 43 0
	testq	%rdi, %rdi
	jne	.L18
.L15:
.LVL10:
	.loc 1 49 0
	rep
	ret
.LFE16:
	.size	list_len_clear, .-list_len_clear
.globl list_sum_clear
	.type	list_sum_clear, @function
list_sum_clear:
.LFB17:
	.loc 1 52 0
.LVL11:
	.loc 1 54 0
	movl	$0, %eax
.LVL12:
	testq	%rdi, %rdi
	je	.L22
.L25:
	.loc 1 55 0
	addl	8(%rdi), %eax
	.loc 1 56 0
	movl	$0, 8(%rdi)
	.loc 1 57 0
	movq	(%rdi), %rdi
.LVL13:
	.loc 1 54 0
	testq	%rdi, %rdi
	jne	.L25
.L22:
.LVL14:
	.loc 1 60 0
	rep
	ret
.LFE17:
	.size	list_sum_clear, .-list_sum_clear
.globl list_sum
	.type	list_sum, @function
list_sum:
.LFB18:
	.loc 1 65 0
.LVL15:
	.loc 1 68 0
	movl	$0, %eax
.LVL16:
	testq	%rdi, %rdi
	je	.L29
.L32:
	.loc 1 69 0
	addl	8(%rdi), %eax
	.loc 1 68 0
	movq	(%rdi), %rdi
.LVL17:
	testq	%rdi, %rdi
	jne	.L32
.L29:
.LVL18:
	.loc 1 71 0
	rep
	ret
.LFE18:
	.size	list_sum, .-list_sum
.globl list_sum2
	.type	list_sum2, @function
list_sum2:
.LFB19:
	.loc 1 76 0
.LVL19:
	.loc 1 80 0
	movl	$0, %eax
.LVL20:
	testq	%rdi, %rdi
	je	.L36
.LVL21:
.L37:
	.loc 1 82 0
	movq	(%rdi), %rdx
.LVL22:
	.loc 1 83 0
	addl	8(%rdi), %eax
	.loc 1 80 0
	testq	%rdx, %rdx
	je	.L36
	movq	%rdx, %rdi
	jmp	.L37
.LVL23:
.L36:
.LVL24:
	.loc 1 86 0
	rep
	ret
.LFE19:
	.size	list_sum2, .-list_sum2
.globl pipe_sum2
	.type	pipe_sum2, @function
pipe_sum2:
.LFB20:
	.loc 1 90 0
.LVL25:
	.loc 1 93 0
	movl	$0, %eax
.LVL26:
	testq	%rdi, %rdi
	je	.L41
.LVL27:
.L42:
	.loc 1 94 0
	movq	(%rdi), %rdx
	.loc 1 95 0
	addl	8(%rdi), %eax
	.loc 1 93 0
	testq	%rdx, %rdx
	je	.L41
	movq	%rdx, %rdi
.LVL28:
	jmp	.L42
.L41:
.LVL29:
	.loc 1 99 0
	rep
	ret
.LFE20:
	.size	pipe_sum2, .-pipe_sum2
.globl rlist_len
	.type	rlist_len, @function
rlist_len:
.LFB21:
	.loc 1 102 0
.LVL30:
	subq	$8, %rsp
.LCFI0:
	.loc 1 103 0
	movl	$0, %eax
	testq	%rdi, %rdi
	je	.L46
	.loc 1 105 0
	movq	(%rdi), %rdi
.LVL31:
	call	rlist_len
	addl	$1, %eax
.LVL32:
.L46:
	.loc 1 106 0
	addq	$8, %rsp
	ret
.LFE21:
	.size	rlist_len, .-rlist_len
.globl rlist_sum
	.type	rlist_sum, @function
rlist_sum:
.LFB22:
	.loc 1 109 0
.LVL33:
	pushq	%rbx
.LCFI1:
	.loc 1 110 0
	movl	$0, %eax
	testq	%rdi, %rdi
	je	.L50
	.loc 1 112 0
	movl	8(%rdi), %ebx
	movq	(%rdi), %rdi
.LVL34:
	call	rlist_sum
	addl	%ebx, %eax
.LVL35:
.L50:
	.loc 1 113 0
	popq	%rbx
	ret
.LFE22:
	.size	rlist_sum, .-rlist_sum
.globl list_copy
	.type	list_copy, @function
list_copy:
.LFB23:
	.loc 1 117 0
.LVL36:
	.loc 1 118 0
	testq	%rdi, %rdi
	je	.L56
	testq	%rsi, %rsi
	je	.L56
.L57:
	.loc 1 119 0
	movl	8(%rdi), %eax
	movl	%eax, 8(%rsi)
	.loc 1 120 0
	movq	(%rdi), %rdi
.LVL37:
	.loc 1 121 0
	movq	(%rsi), %rsi
.LVL38:
	.loc 1 118 0
	testq	%rdi, %rdi
	je	.L56
	testq	%rsi, %rsi
	jne	.L57
.L56:
	rep
	ret
.LFE23:
	.size	list_copy, .-list_copy
.globl plist_copy
	.type	plist_copy, @function
plist_copy:
.LFB24:
	.loc 1 127 0
.LVL39:
	.loc 1 130 0
	testq	%rdi, %rdi
	je	.L62
	testq	%rsi, %rsi
	je	.L62
.LVL40:
.L63:
	.loc 1 134 0
	movq	(%rsi), %rcx
.LVL41:
	.loc 1 136 0
	movq	(%rdi), %rdx
.LVL42:
	.loc 1 137 0
	movl	8(%rdi), %eax
	movl	%eax, 8(%rsi)
	.loc 1 138 0
	testq	%rdx, %rdx
	je	.L62
.LVL43:
	testq	%rcx, %rcx
	je	.L62
.LVL44:
	movq	%rdx, %rdi
	movq	%rcx, %rsi
	jmp	.L63
.LVL45:
.L62:
	rep
	ret
.LFE24:
	.size	plist_copy, .-plist_copy
.globl plist_copy2
	.type	plist_copy2, @function
plist_copy2:
.LFB25:
	.loc 1 143 0
.LVL46:
	.loc 1 145 0
	testq	%rdi, %rdi
	je	.L67
	testq	%rsi, %rsi
	je	.L67
.LVL47:
.L68:
	.loc 1 146 0
	movq	(%rdi), %rdx
	.loc 1 147 0
	movq	(%rsi), %rcx
	.loc 1 148 0
	movl	8(%rdi), %eax
	movl	%eax, 8(%rsi)
	.loc 1 145 0
	testq	%rdx, %rdx
	je	.L67
	testq	%rcx, %rcx
	je	.L67
	movq	%rdx, %rdi
.LVL48:
	movq	%rcx, %rsi
.LVL49:
	jmp	.L68
.L67:
	rep
	ret
.LFE25:
	.size	plist_copy2, .-plist_copy2
.globl stationary_copy
	.type	stationary_copy, @function
stationary_copy:
.LFB26:
	.loc 1 155 0
.LVL50:
	.loc 1 155 0
	movl	$0, %edx
.LVL51:
.L70:
	.loc 1 158 0
	movl	8(%rdi), %eax
	movl	%eax, 8(%rsi)
	.loc 1 157 0
	addl	$1, %edx
	cmpl	$1024, %edx
	jne	.L70
	.loc 1 160 0
	rep
	ret
.LFE26:
	.size	stationary_copy, .-stationary_copy
	.type	test, @function
test:
.LFB27:
	.loc 1 183 0
.LVL52:
	movq	%rbx, -24(%rsp)
.LCFI2:
	movq	%rbp, -16(%rsp)
.LCFI3:
	movq	%r12, -8(%rsp)
.LCFI4:
	subq	$24, %rsp
.LCFI5:
	.loc 1 185 0
	leal	-1(%rdi), %eax
	cltq
	movl	$main_list, %ebx
	movq	%rax, %rbp
	salq	$4, %rbp
	movq	(%rbp,%rbx), %r12
.LVL53:
	.loc 1 186 0
	movq	$0, (%rbp,%rbx)
	.loc 1 187 0
	movslq	myindex(%rip),%rax
	movq	%rbx, %rdi
.LVL54:
	call	*test_fun(,%rax,8)
	.loc 1 188 0
	movq	%r12, (%rbp,%rbx)
	.loc 1 189 0
	movq	(%rsp), %rbx
	movq	8(%rsp), %rbp
	movq	16(%rsp), %r12
.LVL55:
	addq	$24, %rsp
	ret
.LFE27:
	.size	test, .-test
	.type	test2a, @function
test2a:
.LFB29:
	.loc 1 217 0
.LVL56:
	movq	%rbx, -24(%rsp)
.LCFI6:
	movq	%rbp, -16(%rsp)
.LCFI7:
	movq	%r12, -8(%rsp)
.LCFI8:
	subq	$24, %rsp
.LCFI9:
	.loc 1 218 0
	leal	-1(%rdi), %eax
	cltq
	movl	$main_list, %ebx
	movq	%rax, %rbp
	salq	$4, %rbp
	movq	(%rbp,%rbx), %r12
.LVL57:
	.loc 1 219 0
	movq	$0, (%rbp,%rbx)
	.loc 1 220 0
	movslq	myindex(%rip),%rax
	movq	%rbx, %rsi
	movq	%rbx, %rdi
.LVL58:
	call	*test2_fun(,%rax,8)
	.loc 1 221 0
	movq	%r12, (%rbp,%rbx)
	.loc 1 222 0
	movq	(%rsp), %rbx
	movq	8(%rsp), %rbp
	movq	16(%rsp), %r12
.LVL59:
	addq	$24, %rsp
	ret
.LFE29:
	.size	test2a, .-test2a
	.type	test2b, @function
test2b:
.LFB30:
	.loc 1 225 0
.LVL60:
	movq	%rbx, -24(%rsp)
.LCFI10:
	movq	%rbp, -16(%rsp)
.LCFI11:
	movq	%r12, -8(%rsp)
.LCFI12:
	subq	$24, %rsp
.LCFI13:
	.loc 1 226 0
	leal	-1(%rdi), %eax
	cltq
	movl	$main_list, %ebx
	movq	%rax, %rbp
	salq	$4, %rbp
	movq	(%rbp,%rbx), %r12
.LVL61:
	.loc 1 227 0
	movq	$0, (%rbp,%rbx)
	.loc 1 228 0
	movslq	myindex(%rip),%rax
	movq	(%rbx), %rsi
	movq	%rbx, %rdi
.LVL62:
	call	*test2_fun(,%rax,8)
	.loc 1 229 0
	movq	%r12, (%rbp,%rbx)
	.loc 1 230 0
	movq	(%rsp), %rbx
	movq	8(%rsp), %rbp
	movq	16(%rsp), %r12
.LVL63:
	addq	$24, %rsp
	ret
.LFE30:
	.size	test2b, .-test2b
	.type	test2c, @function
test2c:
.LFB31:
	.loc 1 233 0
.LVL64:
	movq	%rbx, -24(%rsp)
.LCFI14:
	movq	%rbp, -16(%rsp)
.LCFI15:
	movq	%r12, -8(%rsp)
.LCFI16:
	subq	$24, %rsp
.LCFI17:
	.loc 1 234 0
	leal	-1(%rdi), %eax
	cltq
	movl	$main_list, %ebx
	movq	%rax, %rbp
	salq	$4, %rbp
	movq	(%rbp,%rbx), %r12
.LVL65:
	.loc 1 235 0
	movq	$0, (%rbp,%rbx)
	.loc 1 236 0
	movslq	myindex(%rip),%rax
	movq	%rbx, %rsi
	movq	(%rbx), %rdi
.LVL66:
	call	*test2_fun(,%rax,8)
	.loc 1 237 0
	movq	%r12, (%rbp,%rbx)
	.loc 1 238 0
	movq	(%rsp), %rbx
	movq	8(%rsp), %rbp
	movq	16(%rsp), %r12
.LVL67:
	addq	$24, %rsp
	ret
.LFE31:
	.size	test2c, .-test2c
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Description: %s, f(%d) = %d, "
.LC1:
	.string	"CPE = %.2f\n"
	.text
	.type	run_test, @function
run_test:
.LFB28:
	.loc 1 194 0
.LVL68:
	pushq	%r12
.LCFI18:
	pushq	%rbx
.LCFI19:
	subq	$8, %rsp
.LCFI20:
	movl	%edi, %r12d
	.loc 1 197 0
	movl	$1024, %edi
.LVL69:
	call	mylink
	.loc 1 198 0
	movslq	%r12d,%rbx
.LVL70:
	movl	$main_list, %edi
	call	*test_fun(,%rbx,8)
.LBB10:
.LBB11:
	.file 2 "/usr/include/bits/stdio2.h"
	.loc 2 105 0
	movq	test_descr(,%rbx,8), %rdx
	movl	%eax, %r8d
	movl	$1024, %ecx
	movl	$.LC0, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
.LBE11:
.LBE10:
	.loc 1 200 0
	movl	%r12d, myindex(%rip)
	.loc 1 201 0
	movl	$1024, %esi
	movl	$test, %edi
	call	find_cpe
.LBB12:
.LBB13:
	.loc 2 105 0
	movl	$.LC1, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE13:
.LBE12:
	.loc 1 204 0
	addq	$8, %rsp
	popq	%rbx
.LVL71:
	popq	%r12
.LVL72:
	ret
.LFE28:
	.size	run_test, .-run_test
	.section	.rodata.str1.1
.LC2:
	.string	"Description: %s, %s, "
	.text
	.type	run_test2, @function
run_test2:
.LFB32:
	.loc 1 254 0
.LVL73:
	pushq	%r12
.LCFI21:
	pushq	%rbx
.LCFI22:
	subq	$8, %rsp
.LCFI23:
	movl	%edi, %r12d
	movl	%esi, %ebx
	.loc 1 257 0
	movl	$1024, %edi
.LVL74:
	call	mylink
.LVL75:
.LBB14:
.LBB15:
	.loc 2 105 0
	movslq	%ebx,%rbx
.LVL76:
	movq	case2_descr(,%rbx,8), %rcx
	movslq	%r12d,%rax
.LVL77:
	movq	test2_descr(,%rax,8), %rdx
	movl	$.LC2, %esi
	movl	$1, %edi
	movl	$0, %eax
.LVL78:
	call	__printf_chk
.LBE15:
.LBE14:
	.loc 1 260 0
	movl	%r12d, myindex(%rip)
.LVL79:
	.loc 1 261 0
	movq	case_fun(,%rbx,8), %rdi
	movl	$1024, %esi
	call	find_cpe
.LBB16:
.LBB17:
	.loc 2 105 0
	movl	$.LC1, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE17:
.LBE16:
	.loc 1 264 0
	addq	$8, %rsp
	popq	%rbx
.LVL80:
	popq	%r12
.LVL81:
	ret
.LFE32:
	.size	run_test2, .-run_test2
.globl main
	.type	main, @function
main:
.LFB33:
	.loc 1 267 0
	pushq	%rbx
.LCFI24:
	.loc 1 267 0
	movl	$0, %ebx
.LVL82:
.L86:
	.loc 1 270 0
	movl	%ebx, %edi
	call	run_test
	.loc 1 269 0
	addl	$1, %ebx
	cmpl	$8, %ebx
	jne	.L86
	jmp	.L95
.L96:
	.loc 1 277 0
	movl	$0, %eax
	popq	%rbx
.LVL83:
	.p2align 4,,4
	.p2align 3
	ret
.LVL84:
.L95:
	.loc 1 269 0
	movl	$0, %ebx
.LVL85:
.L88:
	.loc 1 274 0
	movl	$0, %esi
	movl	%ebx, %edi
	call	run_test2
	movl	$1, %esi
	movl	%ebx, %edi
	call	run_test2
	movl	$2, %esi
	movl	%ebx, %edi
	call	run_test2
	.loc 1 272 0
	addl	$1, %ebx
	cmpl	$4, %ebx
	jne	.L88
	jmp	.L96
.LFE33:
	.size	main, .-main
	.section	.rodata
	.align 32
	.type	test_fun, @object
	.size	test_fun, 64
test_fun:
	.quad	list_len
	.quad	list_sum
	.quad	list_sum2
	.quad	pipe_sum2
	.quad	rlist_len
	.quad	rlist_sum
	.quad	list_len_clear
	.quad	list_sum_clear
	.section	.rodata.str1.1
.LC3:
	.string	"Iterative List Length"
.LC4:
	.string	"Iterative List Sum"
.LC5:
	.string	"Pipelined List Sum with old"
.LC6:
	.string	"Pipelined List Sum with new"
.LC7:
	.string	"Recursive List Length"
.LC8:
	.string	"Recursive List Sum"
.LC9:
	.string	"List Length with Clear"
.LC10:
	.string	"List Sum with Clear"
	.section	.rodata
	.align 32
	.type	test_descr, @object
	.size	test_descr, 64
test_descr:
	.quad	.LC3
	.quad	.LC4
	.quad	.LC5
	.quad	.LC6
	.quad	.LC7
	.quad	.LC8
	.quad	.LC9
	.quad	.LC10
	.local	myindex
	.comm	myindex,4,4
	.section	.rodata.str1.1
.LC11:
	.string	"list --> list"
.LC12:
	.string	"list --> list->next"
.LC13:
	.string	"list->next --> list"
	.section	.rodata
	.align 16
	.type	case2_descr, @object
	.size	case2_descr, 24
case2_descr:
	.quad	.LC11
	.quad	.LC12
	.quad	.LC13
	.section	.rodata.str1.1
.LC14:
	.string	"List Copy"
.LC15:
	.string	"Pipelined List Copy Do-While"
.LC16:
	.string	"Pipelined List Copy While"
.LC17:
	.string	"Stationary Copy"
	.section	.rodata
	.align 32
	.type	test2_descr, @object
	.size	test2_descr, 32
test2_descr:
	.quad	.LC14
	.quad	.LC15
	.quad	.LC16
	.quad	.LC17
	.align 16
	.type	case_fun, @object
	.size	case_fun, 24
case_fun:
	.quad	test2a
	.quad	test2b
	.quad	test2c
	.align 32
	.type	test2_fun, @object
	.size	test2_fun, 32
test2_fun:
	.quad	list_copy
	.quad	plist_copy
	.quad	plist_copy2
	.quad	stationary_copy
	.comm	main_list,16384,32
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
	.quad	.LFB14
	.quad	.LFE14-.LFB14
	.align 8
.LEFDE0:
.LSFDE2:
	.long	.LEFDE2-.LASFDE2
.LASFDE2:
	.long	.Lframe0
	.quad	.LFB15
	.quad	.LFE15-.LFB15
	.align 8
.LEFDE2:
.LSFDE4:
	.long	.LEFDE4-.LASFDE4
.LASFDE4:
	.long	.Lframe0
	.quad	.LFB16
	.quad	.LFE16-.LFB16
	.align 8
.LEFDE4:
.LSFDE6:
	.long	.LEFDE6-.LASFDE6
.LASFDE6:
	.long	.Lframe0
	.quad	.LFB17
	.quad	.LFE17-.LFB17
	.align 8
.LEFDE6:
.LSFDE8:
	.long	.LEFDE8-.LASFDE8
.LASFDE8:
	.long	.Lframe0
	.quad	.LFB18
	.quad	.LFE18-.LFB18
	.align 8
.LEFDE8:
.LSFDE10:
	.long	.LEFDE10-.LASFDE10
.LASFDE10:
	.long	.Lframe0
	.quad	.LFB19
	.quad	.LFE19-.LFB19
	.align 8
.LEFDE10:
.LSFDE12:
	.long	.LEFDE12-.LASFDE12
.LASFDE12:
	.long	.Lframe0
	.quad	.LFB20
	.quad	.LFE20-.LFB20
	.align 8
.LEFDE12:
.LSFDE14:
	.long	.LEFDE14-.LASFDE14
.LASFDE14:
	.long	.Lframe0
	.quad	.LFB21
	.quad	.LFE21-.LFB21
	.byte	0x4
	.long	.LCFI0-.LFB21
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE14:
.LSFDE16:
	.long	.LEFDE16-.LASFDE16
.LASFDE16:
	.long	.Lframe0
	.quad	.LFB22
	.quad	.LFE22-.LFB22
	.byte	0x4
	.long	.LCFI1-.LFB22
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE16:
.LSFDE18:
	.long	.LEFDE18-.LASFDE18
.LASFDE18:
	.long	.Lframe0
	.quad	.LFB23
	.quad	.LFE23-.LFB23
	.align 8
.LEFDE18:
.LSFDE20:
	.long	.LEFDE20-.LASFDE20
.LASFDE20:
	.long	.Lframe0
	.quad	.LFB24
	.quad	.LFE24-.LFB24
	.align 8
.LEFDE20:
.LSFDE22:
	.long	.LEFDE22-.LASFDE22
.LASFDE22:
	.long	.Lframe0
	.quad	.LFB25
	.quad	.LFE25-.LFB25
	.align 8
.LEFDE22:
.LSFDE24:
	.long	.LEFDE24-.LASFDE24
.LASFDE24:
	.long	.Lframe0
	.quad	.LFB26
	.quad	.LFE26-.LFB26
	.align 8
.LEFDE24:
.LSFDE26:
	.long	.LEFDE26-.LASFDE26
.LASFDE26:
	.long	.Lframe0
	.quad	.LFB27
	.quad	.LFE27-.LFB27
	.byte	0x4
	.long	.LCFI5-.LFB27
	.byte	0xe
	.uleb128 0x20
	.byte	0x8c
	.uleb128 0x2
	.byte	0x86
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE26:
.LSFDE28:
	.long	.LEFDE28-.LASFDE28
.LASFDE28:
	.long	.Lframe0
	.quad	.LFB29
	.quad	.LFE29-.LFB29
	.byte	0x4
	.long	.LCFI9-.LFB29
	.byte	0xe
	.uleb128 0x20
	.byte	0x8c
	.uleb128 0x2
	.byte	0x86
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE28:
.LSFDE30:
	.long	.LEFDE30-.LASFDE30
.LASFDE30:
	.long	.Lframe0
	.quad	.LFB30
	.quad	.LFE30-.LFB30
	.byte	0x4
	.long	.LCFI13-.LFB30
	.byte	0xe
	.uleb128 0x20
	.byte	0x8c
	.uleb128 0x2
	.byte	0x86
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE30:
.LSFDE32:
	.long	.LEFDE32-.LASFDE32
.LASFDE32:
	.long	.Lframe0
	.quad	.LFB31
	.quad	.LFE31-.LFB31
	.byte	0x4
	.long	.LCFI17-.LFB31
	.byte	0xe
	.uleb128 0x20
	.byte	0x8c
	.uleb128 0x2
	.byte	0x86
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE32:
.LSFDE34:
	.long	.LEFDE34-.LASFDE34
.LASFDE34:
	.long	.Lframe0
	.quad	.LFB28
	.quad	.LFE28-.LFB28
	.byte	0x4
	.long	.LCFI18-.LFB28
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI19-.LCFI18
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI20-.LCFI19
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.align 8
.LEFDE34:
.LSFDE36:
	.long	.LEFDE36-.LASFDE36
.LASFDE36:
	.long	.Lframe0
	.quad	.LFB32
	.quad	.LFE32-.LFB32
	.byte	0x4
	.long	.LCFI21-.LFB32
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI22-.LCFI21
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI23-.LCFI22
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.align 8
.LEFDE36:
.LSFDE38:
	.long	.LEFDE38-.LASFDE38
.LASFDE38:
	.long	.Lframe0
	.quad	.LFB33
	.quad	.LFE33-.LFB33
	.byte	0x4
	.long	.LCFI24-.LFB33
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE38:
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
	.long	.LFB14
	.long	.LFE14-.LFB14
	.uleb128 0x0
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB15
	.long	.LFE15-.LFB15
	.uleb128 0x0
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB16
	.long	.LFE16-.LFB16
	.uleb128 0x0
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB17
	.long	.LFE17-.LFB17
	.uleb128 0x0
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB18
	.long	.LFE18-.LFB18
	.uleb128 0x0
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB19
	.long	.LFE19-.LFB19
	.uleb128 0x0
	.align 8
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.long	.LFB20
	.long	.LFE20-.LFB20
	.uleb128 0x0
	.align 8
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.long	.LFB21
	.long	.LFE21-.LFB21
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB21
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE15:
.LSFDE17:
	.long	.LEFDE17-.LASFDE17
.LASFDE17:
	.long	.LASFDE17-.Lframe1
	.long	.LFB22
	.long	.LFE22-.LFB22
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI1-.LFB22
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE17:
.LSFDE19:
	.long	.LEFDE19-.LASFDE19
.LASFDE19:
	.long	.LASFDE19-.Lframe1
	.long	.LFB23
	.long	.LFE23-.LFB23
	.uleb128 0x0
	.align 8
.LEFDE19:
.LSFDE21:
	.long	.LEFDE21-.LASFDE21
.LASFDE21:
	.long	.LASFDE21-.Lframe1
	.long	.LFB24
	.long	.LFE24-.LFB24
	.uleb128 0x0
	.align 8
.LEFDE21:
.LSFDE23:
	.long	.LEFDE23-.LASFDE23
.LASFDE23:
	.long	.LASFDE23-.Lframe1
	.long	.LFB25
	.long	.LFE25-.LFB25
	.uleb128 0x0
	.align 8
.LEFDE23:
.LSFDE25:
	.long	.LEFDE25-.LASFDE25
.LASFDE25:
	.long	.LASFDE25-.Lframe1
	.long	.LFB26
	.long	.LFE26-.LFB26
	.uleb128 0x0
	.align 8
.LEFDE25:
.LSFDE27:
	.long	.LEFDE27-.LASFDE27
.LASFDE27:
	.long	.LASFDE27-.Lframe1
	.long	.LFB27
	.long	.LFE27-.LFB27
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI5-.LFB27
	.byte	0xe
	.uleb128 0x20
	.byte	0x8c
	.uleb128 0x2
	.byte	0x86
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE27:
.LSFDE29:
	.long	.LEFDE29-.LASFDE29
.LASFDE29:
	.long	.LASFDE29-.Lframe1
	.long	.LFB29
	.long	.LFE29-.LFB29
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI9-.LFB29
	.byte	0xe
	.uleb128 0x20
	.byte	0x8c
	.uleb128 0x2
	.byte	0x86
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE29:
.LSFDE31:
	.long	.LEFDE31-.LASFDE31
.LASFDE31:
	.long	.LASFDE31-.Lframe1
	.long	.LFB30
	.long	.LFE30-.LFB30
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI13-.LFB30
	.byte	0xe
	.uleb128 0x20
	.byte	0x8c
	.uleb128 0x2
	.byte	0x86
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE31:
.LSFDE33:
	.long	.LEFDE33-.LASFDE33
.LASFDE33:
	.long	.LASFDE33-.Lframe1
	.long	.LFB31
	.long	.LFE31-.LFB31
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI17-.LFB31
	.byte	0xe
	.uleb128 0x20
	.byte	0x8c
	.uleb128 0x2
	.byte	0x86
	.uleb128 0x3
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE33:
.LSFDE35:
	.long	.LEFDE35-.LASFDE35
.LASFDE35:
	.long	.LASFDE35-.Lframe1
	.long	.LFB28
	.long	.LFE28-.LFB28
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI18-.LFB28
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI19-.LCFI18
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI20-.LCFI19
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.align 8
.LEFDE35:
.LSFDE37:
	.long	.LEFDE37-.LASFDE37
.LASFDE37:
	.long	.LASFDE37-.Lframe1
	.long	.LFB32
	.long	.LFE32-.LFB32
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI21-.LFB32
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI22-.LCFI21
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI23-.LCFI22
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.align 8
.LEFDE37:
.LSFDE39:
	.long	.LEFDE39-.LASFDE39
.LASFDE39:
	.long	.LASFDE39-.Lframe1
	.long	.LFB33
	.long	.LFE33-.LFB33
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI24-.LFB33
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE39:
	.text
.Letext0:
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST1:
	.quad	.LVL1-.Ltext0
	.quad	.LVL2-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	0x0
	.quad	0x0
.LLST3:
	.quad	.LVL4-.Ltext0
	.quad	.LVL6-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	0x0
	.quad	0x0
.LLST5:
	.quad	.LVL8-.Ltext0
	.quad	.LVL10-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	0x0
	.quad	0x0
.LLST7:
	.quad	.LVL12-.Ltext0
	.quad	.LVL14-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	0x0
	.quad	0x0
.LLST9:
	.quad	.LVL16-.Ltext0
	.quad	.LVL18-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	0x0
	.quad	0x0
.LLST11:
	.quad	.LVL19-.Ltext0
	.quad	.LVL22-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL22-.Ltext0
	.quad	.LVL23-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL23-.Ltext0
	.quad	.LFE19-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST12:
	.quad	.LVL20-.Ltext0
	.quad	.LVL24-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	0x0
	.quad	0x0
.LLST14:
	.quad	.LVL26-.Ltext0
	.quad	.LVL29-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	0x0
	.quad	0x0
.LLST15:
	.quad	.LFB21-.Ltext0
	.quad	.LCFI0-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI0-.Ltext0
	.quad	.LFE21-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST16:
	.quad	.LVL30-.Ltext0
	.quad	.LVL31-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL32-.Ltext0
	.quad	.LFE21-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST17:
	.quad	.LFB22-.Ltext0
	.quad	.LCFI1-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI1-.Ltext0
	.quad	.LFE22-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST18:
	.quad	.LVL33-.Ltext0
	.quad	.LVL34-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL35-.Ltext0
	.quad	.LFE22-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST21:
	.quad	.LVL39-.Ltext0
	.quad	.LVL42-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL42-.Ltext0
	.quad	.LVL45-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL45-.Ltext0
	.quad	.LFE24-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST22:
	.quad	.LVL39-.Ltext0
	.quad	.LVL41-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL41-.Ltext0
	.quad	.LVL43-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	.LVL43-.Ltext0
	.quad	.LVL44-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL44-.Ltext0
	.quad	.LVL45-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	.LVL45-.Ltext0
	.quad	.LFE24-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	0x0
	.quad	0x0
.LLST25:
	.quad	.LFB27-.Ltext0
	.quad	.LCFI5-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI5-.Ltext0
	.quad	.LFE27-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 32
	.quad	0x0
	.quad	0x0
.LLST26:
	.quad	.LVL52-.Ltext0
	.quad	.LVL54-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST27:
	.quad	.LVL53-.Ltext0
	.quad	.LVL55-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	0x0
	.quad	0x0
.LLST28:
	.quad	.LFB29-.Ltext0
	.quad	.LCFI9-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI9-.Ltext0
	.quad	.LFE29-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 32
	.quad	0x0
	.quad	0x0
.LLST29:
	.quad	.LVL56-.Ltext0
	.quad	.LVL58-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST30:
	.quad	.LVL57-.Ltext0
	.quad	.LVL59-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	0x0
	.quad	0x0
.LLST31:
	.quad	.LFB30-.Ltext0
	.quad	.LCFI13-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI13-.Ltext0
	.quad	.LFE30-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 32
	.quad	0x0
	.quad	0x0
.LLST32:
	.quad	.LVL60-.Ltext0
	.quad	.LVL62-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST33:
	.quad	.LVL61-.Ltext0
	.quad	.LVL63-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	0x0
	.quad	0x0
.LLST34:
	.quad	.LFB31-.Ltext0
	.quad	.LCFI17-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI17-.Ltext0
	.quad	.LFE31-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 32
	.quad	0x0
	.quad	0x0
.LLST35:
	.quad	.LVL64-.Ltext0
	.quad	.LVL66-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST36:
	.quad	.LVL65-.Ltext0
	.quad	.LVL67-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	0x0
	.quad	0x0
.LLST37:
	.quad	.LFB28-.Ltext0
	.quad	.LCFI18-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI18-.Ltext0
	.quad	.LCFI19-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI19-.Ltext0
	.quad	.LCFI20-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 24
	.quad	.LCFI20-.Ltext0
	.quad	.LFE28-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 32
	.quad	0x0
	.quad	0x0
.LLST38:
	.quad	.LVL68-.Ltext0
	.quad	.LVL69-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL69-.Ltext0
	.quad	.LVL70-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	.LVL70-.Ltext0
	.quad	.LVL71-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	.LVL71-.Ltext0
	.quad	.LVL72-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	0x0
	.quad	0x0
.LLST39:
	.quad	.LFB32-.Ltext0
	.quad	.LCFI21-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI21-.Ltext0
	.quad	.LCFI22-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI22-.Ltext0
	.quad	.LCFI23-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 24
	.quad	.LCFI23-.Ltext0
	.quad	.LFE32-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 32
	.quad	0x0
	.quad	0x0
.LLST40:
	.quad	.LVL73-.Ltext0
	.quad	.LVL74-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL74-.Ltext0
	.quad	.LVL77-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	.LVL77-.Ltext0
	.quad	.LVL78-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL79-.Ltext0
	.quad	.LVL81-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	0x0
	.quad	0x0
.LLST41:
	.quad	.LVL73-.Ltext0
	.quad	.LVL75-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL75-.Ltext0
	.quad	.LVL80-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	0x0
	.quad	0x0
.LLST42:
	.quad	.LFB33-.Ltext0
	.quad	.LCFI24-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI24-.Ltext0
	.quad	.LFE33-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST43:
	.quad	.LVL82-.Ltext0
	.quad	.LVL83-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	.LVL84-.Ltext0
	.quad	.LFE33-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	0x0
	.quad	0x0
	.section	.debug_info
	.long	0x82e
	.value	0x2
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF49
	.byte	0x1
	.long	.LASF50
	.long	.LASF51
	.quad	.Ltext0
	.quad	.Letext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF0
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF1
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF2
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF3
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF4
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.long	.LASF5
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF6
	.uleb128 0x4
	.byte	0x8
	.byte	0x7
	.uleb128 0x5
	.byte	0x8
	.long	0x6e
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF7
	.uleb128 0x5
	.byte	0x8
	.long	0x7b
	.uleb128 0x6
	.long	0x6e
	.uleb128 0x5
	.byte	0x8
	.long	0x86
	.uleb128 0x7
	.byte	0x1
	.long	0x92
	.uleb128 0x8
	.long	0x57
	.byte	0x0
	.uleb128 0x9
	.string	"ELE"
	.byte	0x10
	.byte	0x1
	.byte	0x8
	.long	0xbb
	.uleb128 0xa
	.long	.LASF8
	.byte	0x1
	.byte	0x9
	.long	0xbb
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0xa
	.long	.LASF9
	.byte	0x1
	.byte	0xa
	.long	0x57
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.byte	0x0
	.uleb128 0x5
	.byte	0x8
	.long	0x92
	.uleb128 0xb
	.long	.LASF10
	.byte	0x1
	.byte	0xb
	.long	0x92
	.uleb128 0xb
	.long	.LASF11
	.byte	0x1
	.byte	0xb
	.long	0xbb
	.uleb128 0xb
	.long	.LASF12
	.byte	0x1
	.byte	0xa2
	.long	0xe2
	.uleb128 0x5
	.byte	0x8
	.long	0xe8
	.uleb128 0xc
	.byte	0x1
	.long	0x57
	.long	0xf8
	.uleb128 0x8
	.long	0xcc
	.byte	0x0
	.uleb128 0xb
	.long	.LASF13
	.byte	0x1
	.byte	0xcf
	.long	0x103
	.uleb128 0x5
	.byte	0x8
	.long	0x109
	.uleb128 0x7
	.byte	0x1
	.long	0x11a
	.uleb128 0x8
	.long	0xcc
	.uleb128 0x8
	.long	0xcc
	.byte	0x0
	.uleb128 0xb
	.long	.LASF14
	.byte	0x1
	.byte	0xf2
	.long	0x80
	.uleb128 0xd
	.byte	0x1
	.long	.LASF52
	.byte	0x2
	.byte	0x68
	.byte	0x1
	.long	0x57
	.byte	0x2
	.long	0x144
	.uleb128 0xe
	.long	.LASF53
	.byte	0x2
	.byte	0x67
	.long	0x75
	.uleb128 0xf
	.byte	0x0
	.uleb128 0x10
	.byte	0x1
	.long	.LASF23
	.byte	0x1
	.byte	0x12
	.byte	0x1
	.quad	.LFB14
	.quad	.LFE14
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x17f
	.uleb128 0x11
	.string	"len"
	.byte	0x1
	.byte	0x11
	.long	0x57
	.byte	0x1
	.byte	0x55
	.uleb128 0x12
	.string	"i"
	.byte	0x1
	.byte	0x13
	.long	0x57
	.long	.LLST1
	.byte	0x0
	.uleb128 0x13
	.byte	0x1
	.long	.LASF15
	.byte	0x1
	.byte	0x1e
	.byte	0x1
	.long	0x57
	.quad	.LFB15
	.quad	.LFE15
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x1bf
	.uleb128 0x11
	.string	"ls"
	.byte	0x1
	.byte	0x1e
	.long	0xcc
	.byte	0x1
	.byte	0x55
	.uleb128 0x12
	.string	"len"
	.byte	0x1
	.byte	0x1f
	.long	0x57
	.long	.LLST3
	.byte	0x0
	.uleb128 0x13
	.byte	0x1
	.long	.LASF16
	.byte	0x1
	.byte	0x29
	.byte	0x1
	.long	0x57
	.quad	.LFB16
	.quad	.LFE16
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x1ff
	.uleb128 0x11
	.string	"ls"
	.byte	0x1
	.byte	0x28
	.long	0xcc
	.byte	0x1
	.byte	0x55
	.uleb128 0x12
	.string	"len"
	.byte	0x1
	.byte	0x2a
	.long	0x57
	.long	.LLST5
	.byte	0x0
	.uleb128 0x13
	.byte	0x1
	.long	.LASF17
	.byte	0x1
	.byte	0x34
	.byte	0x1
	.long	0x57
	.quad	.LFB17
	.quad	.LFE17
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x23f
	.uleb128 0x11
	.string	"ls"
	.byte	0x1
	.byte	0x33
	.long	0xcc
	.byte	0x1
	.byte	0x55
	.uleb128 0x12
	.string	"sum"
	.byte	0x1
	.byte	0x35
	.long	0x57
	.long	.LLST7
	.byte	0x0
	.uleb128 0x13
	.byte	0x1
	.long	.LASF18
	.byte	0x1
	.byte	0x41
	.byte	0x1
	.long	0x57
	.quad	.LFB18
	.quad	.LFE18
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x27f
	.uleb128 0x11
	.string	"ls"
	.byte	0x1
	.byte	0x40
	.long	0xcc
	.byte	0x1
	.byte	0x55
	.uleb128 0x12
	.string	"sum"
	.byte	0x1
	.byte	0x42
	.long	0x57
	.long	.LLST9
	.byte	0x0
	.uleb128 0x13
	.byte	0x1
	.long	.LASF19
	.byte	0x1
	.byte	0x4c
	.byte	0x1
	.long	0x57
	.quad	.LFB19
	.quad	.LFE19
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x2cc
	.uleb128 0x14
	.string	"ls"
	.byte	0x1
	.byte	0x4b
	.long	0xcc
	.long	.LLST11
	.uleb128 0x12
	.string	"sum"
	.byte	0x1
	.byte	0x4d
	.long	0x57
	.long	.LLST12
	.uleb128 0x15
	.string	"old"
	.byte	0x1
	.byte	0x4e
	.long	0xcc
	.byte	0x0
	.uleb128 0x13
	.byte	0x1
	.long	.LASF20
	.byte	0x1
	.byte	0x5a
	.byte	0x1
	.long	0x57
	.quad	.LFB20
	.quad	.LFE20
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x319
	.uleb128 0x11
	.string	"ls"
	.byte	0x1
	.byte	0x59
	.long	0xcc
	.byte	0x1
	.byte	0x55
	.uleb128 0x12
	.string	"sum"
	.byte	0x1
	.byte	0x5b
	.long	0x57
	.long	.LLST14
	.uleb128 0x16
	.long	.LASF8
	.byte	0x1
	.byte	0x5c
	.long	0xcc
	.byte	0x1
	.byte	0x51
	.byte	0x0
	.uleb128 0x17
	.byte	0x1
	.long	.LASF21
	.byte	0x1
	.byte	0x66
	.byte	0x1
	.long	0x57
	.quad	.LFB21
	.quad	.LFE21
	.long	.LLST15
	.long	0x34d
	.uleb128 0x14
	.string	"ls"
	.byte	0x1
	.byte	0x65
	.long	0xcc
	.long	.LLST16
	.byte	0x0
	.uleb128 0x17
	.byte	0x1
	.long	.LASF22
	.byte	0x1
	.byte	0x6d
	.byte	0x1
	.long	0x57
	.quad	.LFB22
	.quad	.LFE22
	.long	.LLST17
	.long	0x381
	.uleb128 0x14
	.string	"ls"
	.byte	0x1
	.byte	0x6c
	.long	0xcc
	.long	.LLST18
	.byte	0x0
	.uleb128 0x10
	.byte	0x1
	.long	.LASF24
	.byte	0x1
	.byte	0x75
	.byte	0x1
	.quad	.LFB23
	.quad	.LFE23
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x3bc
	.uleb128 0x11
	.string	"src"
	.byte	0x1
	.byte	0x74
	.long	0xcc
	.byte	0x1
	.byte	0x55
	.uleb128 0x18
	.long	.LASF25
	.byte	0x1
	.byte	0x74
	.long	0xcc
	.byte	0x1
	.byte	0x54
	.byte	0x0
	.uleb128 0x10
	.byte	0x1
	.long	.LASF26
	.byte	0x1
	.byte	0x7f
	.byte	0x1
	.quad	.LFB24
	.quad	.LFE24
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x411
	.uleb128 0x14
	.string	"src"
	.byte	0x1
	.byte	0x7e
	.long	0xcc
	.long	.LLST21
	.uleb128 0x19
	.long	.LASF25
	.byte	0x1
	.byte	0x7e
	.long	0xcc
	.long	.LLST22
	.uleb128 0x1a
	.long	.LASF27
	.byte	0x1
	.byte	0x80
	.long	0xcc
	.uleb128 0x1a
	.long	.LASF28
	.byte	0x1
	.byte	0x81
	.long	0xcc
	.byte	0x0
	.uleb128 0x10
	.byte	0x1
	.long	.LASF29
	.byte	0x1
	.byte	0x8f
	.byte	0x1
	.quad	.LFB25
	.quad	.LFE25
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x466
	.uleb128 0x11
	.string	"src"
	.byte	0x1
	.byte	0x8e
	.long	0xcc
	.byte	0x1
	.byte	0x55
	.uleb128 0x18
	.long	.LASF25
	.byte	0x1
	.byte	0x8e
	.long	0xcc
	.byte	0x1
	.byte	0x54
	.uleb128 0x16
	.long	.LASF30
	.byte	0x1
	.byte	0x90
	.long	0xcc
	.byte	0x1
	.byte	0x51
	.uleb128 0x16
	.long	.LASF31
	.byte	0x1
	.byte	0x90
	.long	0xcc
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0x10
	.byte	0x1
	.long	.LASF32
	.byte	0x1
	.byte	0x9b
	.byte	0x1
	.quad	.LFB26
	.quad	.LFE26
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x4ac
	.uleb128 0x11
	.string	"src"
	.byte	0x1
	.byte	0x9a
	.long	0xcc
	.byte	0x1
	.byte	0x55
	.uleb128 0x18
	.long	.LASF25
	.byte	0x1
	.byte	0x9a
	.long	0xcc
	.byte	0x1
	.byte	0x54
	.uleb128 0x1b
	.string	"i"
	.byte	0x1
	.byte	0x9c
	.long	0x57
	.byte	0x1
	.byte	0x51
	.byte	0x0
	.uleb128 0x1c
	.long	.LASF34
	.byte	0x1
	.byte	0xb7
	.byte	0x1
	.quad	.LFB27
	.quad	.LFE27
	.long	.LLST25
	.long	0x4e9
	.uleb128 0x14
	.string	"n"
	.byte	0x1
	.byte	0xb6
	.long	0x57
	.long	.LLST26
	.uleb128 0x1d
	.long	.LASF33
	.byte	0x1
	.byte	0xb9
	.long	0xcc
	.long	.LLST27
	.byte	0x0
	.uleb128 0x1c
	.long	.LASF35
	.byte	0x1
	.byte	0xd9
	.byte	0x1
	.quad	.LFB29
	.quad	.LFE29
	.long	.LLST28
	.long	0x526
	.uleb128 0x14
	.string	"n"
	.byte	0x1
	.byte	0xd8
	.long	0x57
	.long	.LLST29
	.uleb128 0x1d
	.long	.LASF33
	.byte	0x1
	.byte	0xda
	.long	0xcc
	.long	.LLST30
	.byte	0x0
	.uleb128 0x1c
	.long	.LASF36
	.byte	0x1
	.byte	0xe1
	.byte	0x1
	.quad	.LFB30
	.quad	.LFE30
	.long	.LLST31
	.long	0x563
	.uleb128 0x14
	.string	"n"
	.byte	0x1
	.byte	0xe0
	.long	0x57
	.long	.LLST32
	.uleb128 0x1d
	.long	.LASF33
	.byte	0x1
	.byte	0xe2
	.long	0xcc
	.long	.LLST33
	.byte	0x0
	.uleb128 0x1c
	.long	.LASF37
	.byte	0x1
	.byte	0xe9
	.byte	0x1
	.quad	.LFB31
	.quad	.LFE31
	.long	.LLST34
	.long	0x5a0
	.uleb128 0x14
	.string	"n"
	.byte	0x1
	.byte	0xe8
	.long	0x57
	.long	.LLST35
	.uleb128 0x1d
	.long	.LASF33
	.byte	0x1
	.byte	0xea
	.long	0xcc
	.long	.LLST36
	.byte	0x0
	.uleb128 0x1c
	.long	.LASF38
	.byte	0x1
	.byte	0xc2
	.byte	0x1
	.quad	.LFB28
	.quad	.LFE28
	.long	.LLST37
	.long	0x622
	.uleb128 0x14
	.string	"i"
	.byte	0x1
	.byte	0xc1
	.long	0x57
	.long	.LLST38
	.uleb128 0x15
	.string	"cpe"
	.byte	0x1
	.byte	0xc3
	.long	0x622
	.uleb128 0x15
	.string	"len"
	.byte	0x1
	.byte	0xc4
	.long	0x57
	.uleb128 0x1e
	.long	0x125
	.quad	.LBB10
	.quad	.LBE10
	.byte	0x1
	.byte	0xc6
	.long	0x604
	.uleb128 0x1f
	.long	0x137
	.byte	0x0
	.uleb128 0x20
	.long	0x125
	.quad	.LBB12
	.quad	.LBE12
	.byte	0x1
	.byte	0xcb
	.uleb128 0x1f
	.long	0x137
	.byte	0x0
	.byte	0x0
	.uleb128 0x2
	.byte	0x8
	.byte	0x4
	.long	.LASF39
	.uleb128 0x1c
	.long	.LASF40
	.byte	0x1
	.byte	0xfe
	.byte	0x1
	.quad	.LFB32
	.quad	.LFE32
	.long	.LLST39
	.long	0x6bb
	.uleb128 0x14
	.string	"i"
	.byte	0x1
	.byte	0xfd
	.long	0x57
	.long	.LLST40
	.uleb128 0x14
	.string	"c"
	.byte	0x1
	.byte	0xfd
	.long	0x57
	.long	.LLST41
	.uleb128 0x15
	.string	"cpe"
	.byte	0x1
	.byte	0xff
	.long	0x622
	.uleb128 0x21
	.string	"len"
	.byte	0x1
	.value	0x100
	.long	0x57
	.uleb128 0x22
	.long	0x125
	.quad	.LBB14
	.quad	.LBE14
	.byte	0x1
	.value	0x102
	.long	0x69c
	.uleb128 0x1f
	.long	0x137
	.byte	0x0
	.uleb128 0x23
	.long	0x125
	.quad	.LBB16
	.quad	.LBE16
	.byte	0x1
	.value	0x107
	.uleb128 0x1f
	.long	0x137
	.byte	0x0
	.byte	0x0
	.uleb128 0x24
	.byte	0x1
	.long	.LASF41
	.byte	0x1
	.value	0x10b
	.long	0x57
	.quad	.LFB33
	.quad	.LFE33
	.long	.LLST42
	.long	0x6f9
	.uleb128 0x25
	.string	"i"
	.byte	0x1
	.value	0x10c
	.long	0x57
	.long	.LLST43
	.uleb128 0x21
	.string	"c"
	.byte	0x1
	.value	0x10c
	.long	0x57
	.byte	0x0
	.uleb128 0x26
	.long	0xd7
	.long	0x709
	.uleb128 0x27
	.long	0x65
	.byte	0x7
	.byte	0x0
	.uleb128 0x16
	.long	.LASF42
	.byte	0x1
	.byte	0xa7
	.long	0x71e
	.byte	0x9
	.byte	0x3
	.quad	test_fun
	.uleb128 0x6
	.long	0x6f9
	.uleb128 0x26
	.long	0x68
	.long	0x733
	.uleb128 0x27
	.long	0x65
	.byte	0x7
	.byte	0x0
	.uleb128 0x16
	.long	.LASF43
	.byte	0x1
	.byte	0xaa
	.long	0x748
	.byte	0x9
	.byte	0x3
	.quad	test_descr
	.uleb128 0x6
	.long	0x723
	.uleb128 0x16
	.long	.LASF44
	.byte	0x1
	.byte	0xb4
	.long	0x57
	.byte	0x9
	.byte	0x3
	.quad	myindex
	.uleb128 0x26
	.long	0xf8
	.long	0x772
	.uleb128 0x27
	.long	0x65
	.byte	0x3
	.byte	0x0
	.uleb128 0x16
	.long	.LASF45
	.byte	0x1
	.byte	0xd2
	.long	0x787
	.byte	0x9
	.byte	0x3
	.quad	test2_fun
	.uleb128 0x6
	.long	0x762
	.uleb128 0x26
	.long	0x68
	.long	0x79c
	.uleb128 0x27
	.long	0x65
	.byte	0x3
	.byte	0x0
	.uleb128 0x16
	.long	.LASF46
	.byte	0x1
	.byte	0xd4
	.long	0x7b1
	.byte	0x9
	.byte	0x3
	.quad	test2_descr
	.uleb128 0x6
	.long	0x78c
	.uleb128 0x26
	.long	0x11a
	.long	0x7c6
	.uleb128 0x27
	.long	0x65
	.byte	0x2
	.byte	0x0
	.uleb128 0x16
	.long	.LASF47
	.byte	0x1
	.byte	0xf4
	.long	0x7db
	.byte	0x9
	.byte	0x3
	.quad	case_fun
	.uleb128 0x6
	.long	0x7b6
	.uleb128 0x26
	.long	0x68
	.long	0x7f0
	.uleb128 0x27
	.long	0x65
	.byte	0x2
	.byte	0x0
	.uleb128 0x16
	.long	.LASF48
	.byte	0x1
	.byte	0xf7
	.long	0x805
	.byte	0x9
	.byte	0x3
	.quad	case2_descr
	.uleb128 0x6
	.long	0x7e0
	.uleb128 0x26
	.long	0xc1
	.long	0x81b
	.uleb128 0x28
	.long	0x65
	.value	0x3ff
	.byte	0x0
	.uleb128 0x29
	.long	.LASF54
	.byte	0x1
	.byte	0xf
	.long	0x80a
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	main_list
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
	.uleb128 0x3
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
	.uleb128 0x4
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x5
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x6
	.uleb128 0x26
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x9
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
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
	.uleb128 0xa
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
	.uleb128 0xb
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
	.uleb128 0xc
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xd
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
	.uleb128 0xe
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
	.uleb128 0xf
	.uleb128 0x18
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0x10
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
	.uleb128 0x11
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
	.uleb128 0x12
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
	.uleb128 0x14
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
	.uleb128 0x15
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
	.byte	0x0
	.byte	0x0
	.uleb128 0x16
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
	.uleb128 0x17
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
	.uleb128 0x18
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
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x19
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
	.uleb128 0x1a
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
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x1c
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
	.uleb128 0x1f
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
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
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x22
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
	.uleb128 0x5
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x23
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
	.uleb128 0x5
	.byte	0x0
	.byte	0x0
	.uleb128 0x24
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
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
	.uleb128 0x25
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x26
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x27
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x28
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x5
	.byte	0x0
	.byte	0x0
	.uleb128 0x29
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
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.section	.debug_pubnames,"",@progbits
	.long	0xe9
	.value	0x2
	.long	.Ldebug_info0
	.long	0x832
	.long	0x144
	.string	"mylink"
	.long	0x17f
	.string	"list_len"
	.long	0x1bf
	.string	"list_len_clear"
	.long	0x1ff
	.string	"list_sum_clear"
	.long	0x23f
	.string	"list_sum"
	.long	0x27f
	.string	"list_sum2"
	.long	0x2cc
	.string	"pipe_sum2"
	.long	0x319
	.string	"rlist_len"
	.long	0x34d
	.string	"rlist_sum"
	.long	0x381
	.string	"list_copy"
	.long	0x3bc
	.string	"plist_copy"
	.long	0x411
	.string	"plist_copy2"
	.long	0x466
	.string	"stationary_copy"
	.long	0x6bb
	.string	"main"
	.long	0x81b
	.string	"main_list"
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
	.section	.debug_str,"MS",@progbits,1
.LASF52:
	.string	"printf"
.LASF32:
	.string	"stationary_copy"
.LASF45:
	.string	"test2_fun"
.LASF43:
	.string	"test_descr"
.LASF34:
	.string	"test"
.LASF8:
	.string	"next"
.LASF4:
	.string	"signed char"
.LASF50:
	.string	"list.c"
.LASF19:
	.string	"list_sum2"
.LASF6:
	.string	"long int"
.LASF39:
	.string	"double"
.LASF42:
	.string	"test_fun"
.LASF47:
	.string	"case_fun"
.LASF49:
	.string	"GNU C 4.3.2"
.LASF28:
	.string	"odest"
.LASF3:
	.string	"unsigned int"
.LASF16:
	.string	"list_len_clear"
.LASF0:
	.string	"long unsigned int"
.LASF18:
	.string	"list_sum"
.LASF48:
	.string	"case2_descr"
.LASF9:
	.string	"data"
.LASF2:
	.string	"short unsigned int"
.LASF54:
	.string	"main_list"
.LASF40:
	.string	"run_test2"
.LASF22:
	.string	"rlist_sum"
.LASF15:
	.string	"list_len"
.LASF23:
	.string	"mylink"
.LASF26:
	.string	"plist_copy"
.LASF24:
	.string	"list_copy"
.LASF25:
	.string	"dest"
.LASF46:
	.string	"test2_descr"
.LASF44:
	.string	"myindex"
.LASF17:
	.string	"list_sum_clear"
.LASF1:
	.string	"unsigned char"
.LASF13:
	.string	"list2_fun"
.LASF27:
	.string	"osrc"
.LASF5:
	.string	"short int"
.LASF21:
	.string	"rlist_len"
.LASF31:
	.string	"ndest"
.LASF10:
	.string	"list_ele"
.LASF12:
	.string	"list_fun"
.LASF35:
	.string	"test2a"
.LASF36:
	.string	"test2b"
.LASF37:
	.string	"test2c"
.LASF7:
	.string	"char"
.LASF30:
	.string	"nsrc"
.LASF11:
	.string	"list_ptr"
.LASF29:
	.string	"plist_copy2"
.LASF14:
	.string	"test_fun2"
.LASF53:
	.string	"__fmt"
.LASF33:
	.string	"onext"
.LASF20:
	.string	"pipe_sum2"
.LASF38:
	.string	"run_test"
.LASF41:
	.string	"main"
.LASF51:
	.string	"/home/rbryant/ics2/code/opt"
	.ident	"GCC: (Ubuntu 4.3.2-1ubuntu11) 4.3.2"
	.section	.note.GNU-stack,"",@progbits
