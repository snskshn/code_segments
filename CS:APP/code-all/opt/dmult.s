	.file	"dmult.c"
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.text
.Ltext0:
.globl list_prod
	.type	list_prod, @function
list_prod:
.LFB20:
	.file 1 "dmult.c"
	.loc 1 52 0
.LVL0:
	.loc 1 54 0
	testq	%rdi, %rdi
	jne	.L2
	movsd	.LC0(%rip), %xmm0
.LVL1:
	ret
.LVL2:
.L2:
	movsd	.LC0(%rip), %xmm0
.LVL3:
.L4:
	.loc 1 55 0
	mulsd	(%rdi), %xmm0
	.loc 1 56 0
	movq	8(%rdi), %rdi
.LVL4:
	.loc 1 54 0
	testq	%rdi, %rdi
	jne	.L4
.LVL5:
	.loc 1 59 0
	rep
	ret
.LFE20:
	.size	list_prod, .-list_prod
.globl wrap_list_prod
	.type	wrap_list_prod, @function
wrap_list_prod:
.LFB21:
	.loc 1 62 0
.LVL6:
	.loc 1 63 0
	movslq	%edi,%rdi
.LVL7:
	movq	list_data(,%rdi,8), %rdi
.LVL8:
	call	list_prod
	.loc 1 64 0
	rep
	ret
.LFE21:
	.size	wrap_list_prod, .-wrap_list_prod
.globl array_u1p1S_prod
	.type	array_u1p1S_prod, @function
array_u1p1S_prod:
.LFB22:
	.loc 1 67 0
.LVL9:
	.loc 1 70 0
	testl	%esi, %esi
	jg	.L10
	movsd	.LC0(%rip), %xmm0
.LVL10:
	ret
.LVL11:
.L10:
	movsd	.LC0(%rip), %xmm0
.LVL12:
	movl	$0, %eax
.L12:
	.loc 1 71 0
	mulsd	(%rdi,%rax,8), %xmm0
	addq	$1, %rax
	.loc 1 70 0
	cmpl	%eax, %esi
	jg	.L12
.LVL13:
	.loc 1 74 0
	rep
	ret
.LFE22:
	.size	array_u1p1S_prod, .-array_u1p1S_prod
.globl wrap_array_u1p1S_prod
	.type	wrap_array_u1p1S_prod, @function
wrap_array_u1p1S_prod:
.LFB23:
	.loc 1 77 0
.LVL14:
	.loc 1 78 0
	movslq	%edi,%rdi
.LVL15:
	movq	array_data(,%rdi,8), %rdi
.LVL16:
	movl	global_cnt(%rip), %esi
	call	array_u1p1S_prod
	.loc 1 79 0
	rep
	ret
.LFE23:
	.size	wrap_array_u1p1S_prod, .-wrap_array_u1p1S_prod
.globl array_u5p5S_prod
	.type	array_u5p5S_prod, @function
array_u5p5S_prod:
.LFB24:
	.loc 1 83 0
.LVL17:
	.loc 1 90 0
	movl	%esi, %ecx
	subl	$5, %ecx
	jns	.L18
	movl	$0, %edx
.LVL18:
	movsd	.LC0(%rip), %xmm0
.LVL19:
	movapd	%xmm0, %xmm4
.LVL20:
	movapd	%xmm0, %xmm3
.LVL21:
	movapd	%xmm0, %xmm2
.LVL22:
	movapd	%xmm0, %xmm1
.LVL23:
	jmp	.L19
.LVL24:
.L18:
	movl	$0, %edx
.LVL25:
	movsd	.LC0(%rip), %xmm0
.LVL26:
	movapd	%xmm0, %xmm4
.LVL27:
	movapd	%xmm0, %xmm3
.LVL28:
	movapd	%xmm0, %xmm2
.LVL29:
	movapd	%xmm0, %xmm1
.LVL30:
.L20:
	.loc 1 91 0
	movslq	%edx,%rax
	mulsd	(%rdi,%rax,8), %xmm0
	.loc 1 92 0
	mulsd	8(%rdi,%rax,8), %xmm4
	.loc 1 93 0
	mulsd	16(%rdi,%rax,8), %xmm3
	.loc 1 94 0
	mulsd	24(%rdi,%rax,8), %xmm2
	.loc 1 95 0
	mulsd	32(%rdi,%rax,8), %xmm1
	.loc 1 90 0
	addl	$5, %edx
	cmpl	%ecx, %edx
	jle	.L20
.L19:
	.loc 1 97 0
	cmpl	%edx, %esi
	jle	.L21
.L24:
	.loc 1 98 0
	movslq	%edx,%rax
.LVL31:
	mulsd	(%rdi,%rax,8), %xmm0
	.loc 1 97 0
	addl	$1, %edx
.LVL32:
	cmpl	%edx, %esi
	jg	.L24
.L21:
	mulsd	%xmm4, %xmm0
.LVL33:
	mulsd	%xmm3, %xmm0
	mulsd	%xmm2, %xmm0
	.loc 1 100 0
	mulsd	%xmm1, %xmm0
	ret
.LFE24:
	.size	array_u5p5S_prod, .-array_u5p5S_prod
.globl wrap_array_u5p5S_prod
	.type	wrap_array_u5p5S_prod, @function
wrap_array_u5p5S_prod:
.LFB25:
	.loc 1 103 0
.LVL34:
	.loc 1 104 0
	movslq	%edi,%rdi
.LVL35:
	movq	array_data(,%rdi,8), %rdi
.LVL36:
	movl	global_cnt(%rip), %esi
	call	array_u5p5S_prod
	.loc 1 105 0
	rep
	ret
.LFE25:
	.size	wrap_array_u5p5S_prod, .-wrap_array_u5p5S_prod
.globl array_u5p1S_prod
	.type	array_u5p1S_prod, @function
array_u5p1S_prod:
.LFB26:
	.loc 1 108 0
.LVL37:
	.loc 1 111 0
	movl	%esi, %ecx
	subl	$5, %ecx
	jns	.L30
	movl	$0, %edx
.LVL38:
	movsd	.LC0(%rip), %xmm0
.LVL39:
	jmp	.L31
.LVL40:
.L30:
	movl	$0, %edx
.LVL41:
	movsd	.LC0(%rip), %xmm0
.LVL42:
.L32:
	.loc 1 112 0
	movslq	%edx,%rax
	mulsd	(%rdi,%rax,8), %xmm0
.LVL43:
	mulsd	8(%rdi,%rax,8), %xmm0
	mulsd	16(%rdi,%rax,8), %xmm0
	mulsd	24(%rdi,%rax,8), %xmm0
	mulsd	32(%rdi,%rax,8), %xmm0
.LVL44:
	.loc 1 111 0
	addl	$5, %edx
	cmpl	%ecx, %edx
	jle	.L32
.L31:
	.loc 1 114 0
	cmpl	%edx, %esi
	jle	.L33
.L36:
	.loc 1 115 0
	movslq	%edx,%rax
.LVL45:
	mulsd	(%rdi,%rax,8), %xmm0
	.loc 1 114 0
	addl	$1, %edx
.LVL46:
	cmpl	%edx, %esi
	jg	.L36
.L33:
.LVL47:
	.loc 1 117 0
	rep
	ret
.LFE26:
	.size	array_u5p1S_prod, .-array_u5p1S_prod
.globl wrap_array_u5p1S_prod
	.type	wrap_array_u5p1S_prod, @function
wrap_array_u5p1S_prod:
.LFB27:
	.loc 1 120 0
.LVL48:
	.loc 1 121 0
	movslq	%edi,%rdi
.LVL49:
	movq	array_data(,%rdi,8), %rdi
.LVL50:
	movl	global_cnt(%rip), %esi
	call	array_u5p1S_prod
	.loc 1 122 0
	rep
	ret
.LFE27:
	.size	wrap_array_u5p1S_prod, .-wrap_array_u5p1S_prod
.globl array_u5p1A_prod
	.type	array_u5p1A_prod, @function
array_u5p1A_prod:
.LFB28:
	.loc 1 126 0
.LVL51:
	.loc 1 129 0
	movl	%esi, %ecx
	subl	$5, %ecx
	jns	.L42
	movl	$0, %edx
.LVL52:
	movsd	.LC0(%rip), %xmm2
.LVL53:
	jmp	.L43
.LVL54:
.L42:
	movl	$0, %edx
.LVL55:
	movsd	.LC0(%rip), %xmm2
.LVL56:
.L44:
	.loc 1 130 0
	movslq	%edx,%rax
	movsd	(%rdi,%rax,8), %xmm1
	mulsd	8(%rdi,%rax,8), %xmm1
	movsd	16(%rdi,%rax,8), %xmm0
	mulsd	24(%rdi,%rax,8), %xmm0
	mulsd	32(%rdi,%rax,8), %xmm0
	mulsd	%xmm0, %xmm1
	mulsd	%xmm1, %xmm2
	.loc 1 129 0
	addl	$5, %edx
	cmpl	%ecx, %edx
	jle	.L44
.L43:
	.loc 1 132 0
	cmpl	%edx, %esi
	jle	.L45
.L48:
	.loc 1 133 0
	movslq	%edx,%rax
.LVL57:
	mulsd	(%rdi,%rax,8), %xmm2
	.loc 1 132 0
	addl	$1, %edx
.LVL58:
	cmpl	%edx, %esi
	jg	.L48
.L45:
	.loc 1 135 0
	movapd	%xmm2, %xmm0
	ret
.LFE28:
	.size	array_u5p1A_prod, .-array_u5p1A_prod
.globl wrap_array_u5p1A_prod
	.type	wrap_array_u5p1A_prod, @function
wrap_array_u5p1A_prod:
.LFB29:
	.loc 1 138 0
.LVL59:
	.loc 1 139 0
	movslq	%edi,%rdi
.LVL60:
	movq	array_data(,%rdi,8), %rdi
.LVL61:
	movl	global_cnt(%rip), %esi
	call	array_u5p1A_prod
	.loc 1 140 0
	rep
	ret
.LFE29:
	.size	wrap_array_u5p1A_prod, .-wrap_array_u5p1A_prod
.globl sse_u1p1_prod
	.type	sse_u1p1_prod, @function
sse_u1p1_prod:
.LFB30:
	.loc 1 156 0
.LVL62:
	movl	%esi, %r8d
	.loc 1 164 0
	movabsq	$4607182418800017408, %rax
	movq	%rax, -24(%rsp)
	movq	%rax, -16(%rsp)
	.loc 1 165 0
	movapd	-24(%rsp), %xmm1
.LVL63:
	.loc 1 168 0
	testb	$15, %dil
	je	.L64
.LVL64:
	testl	%esi, %esi
	jne	.L54
.LVL65:
.L64:
	movq	%rdi, %rsi
.LVL66:
	movsd	.LC0(%rip), %xmm0
.LVL67:
	jmp	.L56
.LVL68:
.L54:
	movq	%rdi, %rsi
.LVL69:
	movsd	.LC0(%rip), %xmm0
.LVL70:
.L57:
	.loc 1 169 0
	mulsd	(%rsi), %xmm0
	addq	$8, %rsi
	.loc 1 170 0
	subl	$1, %r8d
	.loc 1 168 0
	testb	$15, %sil
	je	.L56
	testl	%r8d, %r8d
	jne	.L57
.L56:
	.loc 1 173 0
	cmpl	$1, %r8d
	jle	.L59
	movq	%rsi, %rcx
.LBB11:
	.loc 1 156 0
	movl	%r8d, %edi
.LVL71:
.LBE11:
	.loc 1 173 0
	leal	-2(%r8), %eax
	shrl	%eax
	mov	%eax, %eax
	salq	$4, %rax
	leaq	16(%rax,%rsi), %rdx
.L60:
.LBB12:
	.loc 1 175 0
	mulpd	(%rcx), %xmm1
	.loc 1 176 0
	addq	$16, %rcx
.LVL72:
	.loc 1 156 0
	movl	%edi, %eax
.LBE12:
	.loc 1 173 0
	cmpq	%rdx, %rcx
	jne	.L60
	.loc 1 156 0
	leal	-2(%rax), %edx
	shrl	%edx
	leal	(%rdx,%rdx), %eax
	negl	%eax
	leal	-2(%r8,%rax), %r8d
	mov	%edx, %edx
	salq	$4, %rdx
	leaq	16(%rdx,%rsi), %rsi
.LVL73:
.L59:
	.loc 1 180 0
	testl	%r8d, %r8d
	je	.L61
	movq	%rsi, %rdx
	leal	-1(%r8), %eax
	leaq	8(%rsi,%rax,8), %rax
.L62:
	.loc 1 181 0
	mulsd	(%rdx), %xmm0
	addq	$8, %rdx
.LVL74:
	.loc 1 180 0
	cmpq	%rax, %rdx
	jne	.L62
.LVL75:
.L61:
	.loc 1 184 0
	movapd	%xmm1, -24(%rsp)
	.loc 1 186 0
	mulsd	-24(%rsp), %xmm0
.LVL76:
	.loc 1 188 0
	mulsd	-16(%rsp), %xmm0
.LVL77:
	ret
.LFE30:
	.size	sse_u1p1_prod, .-sse_u1p1_prod
.globl wrap_sse_u1p1_prod
	.type	wrap_sse_u1p1_prod, @function
wrap_sse_u1p1_prod:
.LFB31:
	.loc 1 191 0
.LVL78:
	subq	$8, %rsp
.LCFI0:
	.loc 1 192 0
	movslq	%edi,%rdi
.LVL79:
	movq	array_data(,%rdi,8), %rdi
.LVL80:
	movl	global_cnt(%rip), %esi
	call	sse_u1p1_prod
	.loc 1 193 0
	addq	$8, %rsp
	ret
.LFE31:
	.size	wrap_sse_u1p1_prod, .-wrap_sse_u1p1_prod
.globl sse_u5p5_prod
	.type	sse_u5p5_prod, @function
sse_u5p5_prod:
.LFB32:
	.loc 1 199 0
.LVL81:
	movl	%esi, %r8d
	.loc 1 207 0
	movabsq	$4607182418800017408, %rax
	movq	%rax, -24(%rsp)
	movq	%rax, -16(%rsp)
	.loc 1 208 0
	movapd	-24(%rsp), %xmm2
.LVL82:
	.loc 1 211 0
	testb	$15, %dil
	je	.L82
.LVL83:
	testl	%esi, %esi
	jne	.L71
.LVL84:
.L82:
	movq	%rdi, %rsi
.LVL85:
	movsd	.LC0(%rip), %xmm5
.LVL86:
	jmp	.L73
.LVL87:
.L71:
	movq	%rdi, %rsi
.LVL88:
	movsd	.LC0(%rip), %xmm5
.LVL89:
.L74:
	.loc 1 212 0
	mulsd	(%rsi), %xmm5
	addq	$8, %rsi
	.loc 1 213 0
	subl	$1, %r8d
	.loc 1 211 0
	testb	$15, %sil
	je	.L73
	testl	%r8d, %r8d
	jne	.L74
.L73:
	.loc 1 216 0
	cmpl	$9, %r8d
	jg	.L76
	movapd	%xmm2, %xmm4
.LVL90:
	movapd	%xmm2, %xmm0
.LVL91:
	movapd	%xmm2, %xmm3
.LVL92:
	movapd	%xmm2, %xmm1
.LVL93:
	jmp	.L77
.LVL94:
.L76:
	movapd	%xmm2, %xmm4
.LVL95:
	movapd	%xmm2, %xmm0
.LVL96:
	movapd	%xmm2, %xmm3
.LVL97:
	movapd	%xmm2, %xmm1
.LVL98:
	movq	%rsi, %rcx
.LBB13:
	.loc 1 199 0
	movl	%r8d, %edi
.LVL99:
.LBE13:
	.loc 1 216 0
	leal	-10(%r8), %edx
	movl	$-858993459, %eax
	mull	%edx
	shrl	$3, %edx
	mov	%edx, %edx
	leaq	5(%rdx,%rdx,4), %rdx
	salq	$4, %rdx
	leaq	(%rsi,%rdx), %rdx
.L78:
.LBB14:
	.loc 1 218 0
	mulpd	(%rcx), %xmm4
	.loc 1 220 0
	mulpd	16(%rcx), %xmm0
	.loc 1 222 0
	mulpd	32(%rcx), %xmm3
	.loc 1 224 0
	mulpd	48(%rcx), %xmm1
	.loc 1 226 0
	mulpd	64(%rcx), %xmm2
	.loc 1 227 0
	addq	$80, %rcx
.LVL100:
	.loc 1 199 0
	movl	%edi, %eax
.LBE14:
	.loc 1 216 0
	cmpq	%rdx, %rcx
	jne	.L78
	.loc 1 199 0
	leal	-10(%rax), %edx
	movl	$-858993459, %eax
	mull	%edx
	shrl	$3, %edx
	leal	(%rdx,%rdx,4), %eax
	addl	%eax, %eax
	negl	%eax
	leal	-10(%r8,%rax), %r8d
	mov	%edx, %edx
	leaq	5(%rdx,%rdx,4), %rdx
	salq	$4, %rdx
	addq	%rdx, %rsi
.LVL101:
.L77:
	.loc 1 231 0
	testl	%r8d, %r8d
	je	.L79
	movq	%rsi, %rcx
	leal	-1(%r8), %eax
	leaq	8(%rsi,%rax,8), %rax
.L80:
	.loc 1 232 0
	mulsd	(%rcx), %xmm5
	addq	$8, %rcx
.LVL102:
	.loc 1 231 0
	cmpq	%rax, %rcx
	jne	.L80
.LVL103:
.L79:
	.loc 1 235 0
	mulpd	%xmm4, %xmm0
.LVL104:
	mulpd	%xmm3, %xmm1
.LVL105:
	mulpd	%xmm2, %xmm1
	mulpd	%xmm1, %xmm0
	movapd	%xmm0, -24(%rsp)
	.loc 1 237 0
	movapd	%xmm5, %xmm0
	mulsd	-24(%rsp), %xmm0
.LVL106:
	.loc 1 239 0
	mulsd	-16(%rsp), %xmm0
.LVL107:
	ret
.LFE32:
	.size	sse_u5p5_prod, .-sse_u5p5_prod
.globl wrap_sse_u5p5_prod
	.type	wrap_sse_u5p5_prod, @function
wrap_sse_u5p5_prod:
.LFB33:
	.loc 1 242 0
.LVL108:
	subq	$8, %rsp
.LCFI1:
	.loc 1 243 0
	movslq	%edi,%rdi
.LVL109:
	movq	array_data(,%rdi,8), %rdi
.LVL110:
	movl	global_cnt(%rip), %esi
	call	sse_u5p5_prod
	.loc 1 244 0
	addq	$8, %rsp
	ret
.LFE33:
	.size	wrap_sse_u5p5_prod, .-wrap_sse_u5p5_prod
.globl sse_u8p8_prod
	.type	sse_u8p8_prod, @function
sse_u8p8_prod:
.LFB34:
	.loc 1 249 0
.LVL111:
	movl	%esi, %r8d
	.loc 1 257 0
	movabsq	$4607182418800017408, %rax
	movq	%rax, -24(%rsp)
	movq	%rax, -16(%rsp)
	.loc 1 258 0
	movapd	-24(%rsp), %xmm1
.LVL112:
	.loc 1 261 0
	testb	$15, %dil
	je	.L100
.LVL113:
	testl	%esi, %esi
	jne	.L89
.LVL114:
.L100:
	movq	%rdi, %rsi
.LVL115:
	movsd	.LC0(%rip), %xmm8
.LVL116:
	jmp	.L91
.LVL117:
.L89:
	movq	%rdi, %rsi
.LVL118:
	movsd	.LC0(%rip), %xmm8
.LVL119:
.L92:
	.loc 1 262 0
	mulsd	(%rsi), %xmm8
	addq	$8, %rsi
	.loc 1 263 0
	subl	$1, %r8d
	.loc 1 261 0
	testb	$15, %sil
	je	.L91
	testl	%r8d, %r8d
	jne	.L92
.L91:
	.loc 1 266 0
	cmpl	$15, %r8d
	jg	.L94
	movapd	%xmm1, %xmm7
.LVL120:
	movapd	%xmm1, %xmm2
.LVL121:
	movapd	%xmm1, %xmm6
.LVL122:
	movapd	%xmm1, %xmm0
.LVL123:
	movapd	%xmm1, %xmm5
.LVL124:
	movapd	%xmm1, %xmm4
.LVL125:
	movapd	%xmm1, %xmm3
.LVL126:
	jmp	.L95
.LVL127:
.L94:
	movapd	%xmm1, %xmm7
.LVL128:
	movapd	%xmm1, %xmm2
.LVL129:
	movapd	%xmm1, %xmm6
.LVL130:
	movapd	%xmm1, %xmm0
.LVL131:
	movapd	%xmm1, %xmm5
.LVL132:
	movapd	%xmm1, %xmm4
.LVL133:
	movapd	%xmm1, %xmm3
.LVL134:
	movq	%rsi, %rcx
.LBB15:
	.loc 1 249 0
	movl	%r8d, %edi
.LVL135:
.LBE15:
	.loc 1 266 0
	leal	-16(%r8), %eax
	shrl	$4, %eax
	mov	%eax, %eax
	salq	$7, %rax
	leaq	128(%rax,%rsi), %rdx
.L96:
.LBB16:
	.loc 1 268 0
	mulpd	(%rcx), %xmm7
	.loc 1 270 0
	mulpd	16(%rcx), %xmm2
	.loc 1 272 0
	mulpd	32(%rcx), %xmm6
	.loc 1 274 0
	mulpd	48(%rcx), %xmm0
	.loc 1 276 0
	mulpd	64(%rcx), %xmm5
	.loc 1 278 0
	mulpd	80(%rcx), %xmm4
	.loc 1 280 0
	mulpd	96(%rcx), %xmm3
	.loc 1 282 0
	mulpd	112(%rcx), %xmm1
	.loc 1 283 0
	subq	$-128, %rcx
.LVL136:
	.loc 1 249 0
	movl	%edi, %eax
.LBE16:
	.loc 1 266 0
	cmpq	%rdx, %rcx
	jne	.L96
	.loc 1 249 0
	leal	-16(%rax), %edx
	shrl	$4, %edx
	movl	%edx, %eax
	sall	$4, %eax
	negl	%eax
	leal	-16(%r8,%rax), %r8d
	mov	%edx, %edx
	salq	$7, %rdx
	leaq	128(%rdx,%rsi), %rsi
.LVL137:
.L95:
	.loc 1 287 0
	testl	%r8d, %r8d
	je	.L97
	movq	%rsi, %rdx
	leal	-1(%r8), %eax
	leaq	8(%rsi,%rax,8), %rax
.L98:
	.loc 1 288 0
	mulsd	(%rdx), %xmm8
	addq	$8, %rdx
.LVL138:
	.loc 1 287 0
	cmpq	%rax, %rdx
	jne	.L98
.LVL139:
.L97:
	.loc 1 291 0
	mulpd	%xmm7, %xmm2
.LVL140:
	mulpd	%xmm6, %xmm0
.LVL141:
	mulpd	%xmm0, %xmm2
	movapd	%xmm5, %xmm0
	mulpd	%xmm4, %xmm0
	mulpd	%xmm3, %xmm1
.LVL142:
	mulpd	%xmm1, %xmm0
	mulpd	%xmm0, %xmm2
	movapd	%xmm2, -24(%rsp)
	.loc 1 293 0
	movapd	%xmm8, %xmm0
	mulsd	-24(%rsp), %xmm0
.LVL143:
	.loc 1 295 0
	mulsd	-16(%rsp), %xmm0
.LVL144:
	ret
.LFE34:
	.size	sse_u8p8_prod, .-sse_u8p8_prod
.globl wrap_sse_u8p8_prod
	.type	wrap_sse_u8p8_prod, @function
wrap_sse_u8p8_prod:
.LFB35:
	.loc 1 298 0
.LVL145:
	subq	$8, %rsp
.LCFI2:
	.loc 1 299 0
	movslq	%edi,%rdi
.LVL146:
	movq	array_data(,%rdi,8), %rdi
.LVL147:
	movl	global_cnt(%rip), %esi
	call	sse_u8p8_prod
	.loc 1 300 0
	addq	$8, %rsp
	ret
.LFE35:
	.size	wrap_sse_u8p8_prod, .-wrap_sse_u8p8_prod
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC1:
	.string	"Function '%s' Data Error. Test %d"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC2:
	.string	", Expected %.1f.  Got %.1f\n"
	.section	.rodata.str1.8
	.align 8
.LC4:
	.string	"Function '%s' (%s) %.0f cycles, %.0f nsecs, %.2f cycles/element\n"
	.text
.globl tester
	.type	tester, @function
tester:
.LFB36:
	.loc 1 304 0
.LVL148:
	pushq	%r14
.LCFI3:
	pushq	%r13
.LCFI4:
	pushq	%r12
.LCFI5:
	pushq	%rbp
.LCFI6:
	pushq	%rbx
.LCFI7:
	subq	$16, %rsp
.LCFI8:
	movq	%rdi, %r12
	movq	%rsi, %r13
	movq	%rdx, %r14
	.loc 1 304 0
	movl	$answer+32, %ebp
	movl	$4, %ebx
.LVL149:
.L109:
.LBB17:
	.loc 1 308 0
	movl	%ebx, %edi
	call	*%r12
	movsd	%xmm0, 8(%rsp)
.LVL150:
	.loc 1 309 0
	ucomisd	(%rbp), %xmm0
	jp	.L111
	je	.L107
.L111:
.LBB18:
.LBB19:
	.file 2 "/usr/include/bits/stdio2.h"
	.loc 2 105 0
	movl	%ebx, %ecx
	movq	%r13, %rdx
	movl	$.LC1, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
.LBE19:
.LBE18:
.LBB20:
.LBB21:
	movsd	8(%rsp), %xmm1
	movsd	(%rbp), %xmm0
	movl	$.LC2, %esi
	movl	$1, %edi
	movl	$2, %eax
	call	__printf_chk
.L107:
.LBE21:
.LBE20:
.LBE17:
	.loc 1 307 0
	subl	$1, %ebx
	subq	$8, %rbp
	cmpl	$-1, %ebx
	jne	.L109
	.loc 1 315 0
	movl	$0, %eax
	call	start_counter
	.loc 1 316 0
	movl	$0, %edi
	call	*%r12
	.loc 1 317 0
	movl	$0, %eax
	call	get_counter
	movsd	%xmm0, (%rsp)
.LVL151:
	.loc 1 318 0
	movl	$0, %edi
	call	mhz
.LBB22:
.LBB23:
	.loc 2 105 0
	cvtsi2sd	global_cnt(%rip), %xmm1
	divsd	.LC3(%rip), %xmm0
	movsd	(%rsp), %xmm2
	divsd	%xmm1, %xmm2
	movsd	(%rsp), %xmm1
	divsd	%xmm0, %xmm1
	movsd	(%rsp), %xmm0
	movq	%r14, %rcx
	movq	%r13, %rdx
	movl	$.LC4, %esi
	movl	$1, %edi
	movl	$3, %eax
	call	__printf_chk
.LBE23:
.LBE22:
	.loc 1 322 0
	addq	$16, %rsp
	popq	%rbx
.LVL152:
	popq	%rbp
	popq	%r12
.LVL153:
	popq	%r13
.LVL154:
	popq	%r14
.LVL155:
	ret
.LFE36:
	.size	tester, .-tester
.globl init
	.type	init, @function
init:
.LFB19:
	.loc 1 28 0
.LVL156:
	pushq	%r15
.LCFI9:
	pushq	%r14
.LCFI10:
	pushq	%r13
.LCFI11:
	pushq	%r12
.LCFI12:
	pushq	%rbp
.LCFI13:
	pushq	%rbx
.LCFI14:
	subq	$40, %rsp
.LCFI15:
	movl	%edi, 36(%rsp)
	.loc 1 30 0
	movl	%edi, global_cnt(%rip)
	movl	$0, %ebp
.LBB24:
	.loc 1 35 0
	movslq	%edi,%rax
.LVL157:
	movq	%rax, 24(%rsp)
	.loc 1 37 0
	movl	$list_data, %r15d
.LBB25:
	.loc 1 39 0
	movsd	.LC5(%rip), %xmm0
	movsd	%xmm0, 16(%rsp)
	movsd	.LC0(%rip), %xmm0
	movsd	%xmm0, 8(%rsp)
.LVL158:
.L119:
.LBE25:
	.loc 1 33 0
	movl	$0, answer(%rbp)
	movl	$1072693248, answer+4(%rbp)
	.loc 1 35 0
	movl	$8, %esi
	movq	24(%rsp), %rdi
	call	calloc
	movq	%rax, array_data(%rbp)
	.loc 1 36 0
	movl	$16, %esi
	movq	24(%rsp), %rdi
	call	calloc
	.loc 1 37 0
	movq	$0, (%r15,%rbp)
	.loc 1 38 0
	cmpl	$0, 36(%rsp)
	jle	.L115
.LVL159:
	movq	%rax, %rbx
	movl	$0, %r13d
	movl	$0, %r12d
.LBB26:
	.loc 1 45 0
	movl	$answer, %r14d
.L118:
	.loc 1 39 0
	call	random
	movsd	8(%rsp), %xmm0
.LVL160:
	testb	$1, %al
	je	.L117
	movsd	16(%rsp), %xmm0
.L117:
	.loc 1 40 0
	movq	array_data(%rbp), %rax
	movsd	%xmm0, (%rax,%r12)
	.loc 1 42 0
	movsd	%xmm0, (%rbx)
	.loc 1 43 0
	movq	(%r15,%rbp), %rax
	movq	%rax, 8(%rbx)
	.loc 1 44 0
	movq	%rbx, (%r15,%rbp)
	.loc 1 45 0
	mulsd	(%r14,%rbp), %xmm0
.LVL161:
	movsd	%xmm0, (%r14,%rbp)
.LBE26:
	.loc 1 38 0
	addl	$1, %r13d
	addq	$8, %r12
	addq	$16, %rbx
	cmpl	%r13d, 36(%rsp)
	jg	.L118
.L115:
	addq	$8, %rbp
.LBE24:
	.loc 1 32 0
	cmpq	$40, %rbp
	jne	.L119
	.loc 1 48 0
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
.LVL162:
	popq	%r14
	popq	%r15
	ret
.LFE19:
	.size	init, .-init
	.section	.rodata.str1.1
.LC6:
	.string	"Count = %d, Ghz = %.2f\n"
	.text
.globl run
	.type	run, @function
run:
.LFB37:
	.loc 1 343 0
.LVL163:
	pushq	%rbx
.LCFI16:
	movl	%edi, %ebx
	.loc 1 344 0
	call	init
.LVL164:
	.loc 1 345 0
	movl	$0, %edi
	call	mhz
.LBB27:
.LBB28:
	.loc 2 105 0
	divsd	.LC3(%rip), %xmm0
	movl	%ebx, %edx
	movl	$.LC6, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE28:
.LBE27:
	.loc 1 347 0
	movq	tests(%rip), %rdi
	testq	%rdi, %rdi
	je	.L126
	movl	$tests+16, %ebx
.LVL165:
.L125:
	.loc 1 348 0
	movq	-8(%rbx), %rsi
	movq	(%rbx), %rdx
	call	tester
	.loc 1 347 0
	movq	8(%rbx), %rdi
	addq	$24, %rbx
	testq	%rdi, %rdi
	jne	.L125
.LVL166:
.L126:
	.loc 1 350 0
	popq	%rbx
.LVL167:
	ret
.LFE37:
	.size	run, .-run
.globl main
	.type	main, @function
main:
.LFB38:
	.loc 1 352 0
.LVL168:
	subq	$8, %rsp
.LCFI17:
	.loc 1 354 0
	movl	$20000, %eax
.LVL169:
	cmpl	$1, %edi
	jle	.L130
	.loc 1 355 0
	movq	8(%rsi), %rdi
.LVL170:
	call	atoi
.LVL171:
.L130:
	.loc 1 356 0
	movl	%eax, %edi
.LVL172:
	call	run
.LVL173:
	.loc 1 358 0
	movl	$0, %eax
	addq	$8, %rsp
	ret
.LFE38:
	.size	main, .-main
.globl global_cnt
	.data
	.align 4
	.type	global_cnt, @object
	.size	global_cnt, 4
global_cnt:
	.long	10000
.globl list_prod_descr
	.align 16
	.type	list_prod_descr, @object
	.size	list_prod_descr, 30
list_prod_descr:
	.string	"Traversing Singly-Linked List"
.globl array_u1p1S_prod_descr
	.align 32
	.type	array_u1p1S_prod_descr, @object
	.size	array_u1p1S_prod_descr, 56
array_u1p1S_prod_descr:
	.string	"Array.  Unroll 1x.  Parallel 1x, Standard associativity"
.globl array_u5p5S_prod_descr
	.align 32
	.type	array_u5p5S_prod_descr, @object
	.size	array_u5p5S_prod_descr, 56
array_u5p5S_prod_descr:
	.string	"Array.  Unroll 5x.  Parallel 5x, Standard associativity"
.globl array_u5p1S_prod_descr
	.align 32
	.type	array_u5p1S_prod_descr, @object
	.size	array_u5p1S_prod_descr, 56
array_u5p1S_prod_descr:
	.string	"Array.  Unroll 5x.  Parallel 1x, Standard associativity"
.globl array_u5p1A_prod_descr
	.align 32
	.type	array_u5p1A_prod_descr, @object
	.size	array_u5p1A_prod_descr, 56
array_u5p1A_prod_descr:
	.string	"Array.  Unroll 5x.  Parallel 1x, Modified associativity"
.globl sse_u1p1_prod_descr
	.align 32
	.type	sse_u1p1_prod_descr, @object
	.size	sse_u1p1_prod_descr, 34
sse_u1p1_prod_descr:
	.string	"2x SSE.  Unroll 1x.  Parallel 1x."
.globl sse_u5p5_prod_descr
	.align 32
	.type	sse_u5p5_prod_descr, @object
	.size	sse_u5p5_prod_descr, 34
sse_u5p5_prod_descr:
	.string	"2x SSE.  Unroll 5x.  Parallel 5x."
.globl sse_u8p8_prod_descr
	.align 32
	.type	sse_u8p8_prod_descr, @object
	.size	sse_u8p8_prod_descr, 34
sse_u8p8_prod_descr:
	.string	"2x SSE.  Unroll 8x.  Parallel 8x."
.globl tests
	.section	.rodata.str1.1
.LC7:
	.string	"list"
.LC8:
	.string	"array_u1p1S"
.LC9:
	.string	"array_u5p5S"
.LC10:
	.string	"array_u5p1S"
.LC11:
	.string	"array_u5p1A"
.LC12:
	.string	"sse_u1p1"
.LC13:
	.string	"sse_u5p5"
.LC14:
	.string	"sse_u8p8"
	.data
	.align 32
	.type	tests, @object
	.size	tests, 216
tests:
	.quad	wrap_list_prod
	.quad	.LC7
	.quad	list_prod_descr
	.quad	wrap_array_u1p1S_prod
	.quad	.LC8
	.quad	array_u1p1S_prod_descr
	.quad	wrap_array_u5p5S_prod
	.quad	.LC9
	.quad	array_u5p5S_prod_descr
	.quad	wrap_array_u5p1S_prod
	.quad	.LC10
	.quad	array_u5p1S_prod_descr
	.quad	wrap_array_u5p1A_prod
	.quad	.LC11
	.quad	array_u5p1A_prod_descr
	.quad	wrap_sse_u1p1_prod
	.quad	.LC12
	.quad	sse_u1p1_prod_descr
	.quad	wrap_sse_u5p5_prod
	.quad	.LC13
	.quad	sse_u5p5_prod_descr
	.quad	wrap_sse_u8p8_prod
	.quad	.LC14
	.quad	sse_u8p8_prod_descr
	.quad	0
	.quad	0
	.quad	0
	.comm	array_data,40,32
	.comm	list_data,40,32
	.comm	answer,40,32
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	1072693248
	.align 8
.LC3:
	.long	0
	.long	1083129856
	.align 8
.LC5:
	.long	0
	.long	-1074790400
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
	.quad	.LFB20
	.quad	.LFE20-.LFB20
	.align 8
.LEFDE0:
.LSFDE2:
	.long	.LEFDE2-.LASFDE2
.LASFDE2:
	.long	.Lframe0
	.quad	.LFB21
	.quad	.LFE21-.LFB21
	.align 8
.LEFDE2:
.LSFDE4:
	.long	.LEFDE4-.LASFDE4
.LASFDE4:
	.long	.Lframe0
	.quad	.LFB22
	.quad	.LFE22-.LFB22
	.align 8
.LEFDE4:
.LSFDE6:
	.long	.LEFDE6-.LASFDE6
.LASFDE6:
	.long	.Lframe0
	.quad	.LFB23
	.quad	.LFE23-.LFB23
	.align 8
.LEFDE6:
.LSFDE8:
	.long	.LEFDE8-.LASFDE8
.LASFDE8:
	.long	.Lframe0
	.quad	.LFB24
	.quad	.LFE24-.LFB24
	.align 8
.LEFDE8:
.LSFDE10:
	.long	.LEFDE10-.LASFDE10
.LASFDE10:
	.long	.Lframe0
	.quad	.LFB25
	.quad	.LFE25-.LFB25
	.align 8
.LEFDE10:
.LSFDE12:
	.long	.LEFDE12-.LASFDE12
.LASFDE12:
	.long	.Lframe0
	.quad	.LFB26
	.quad	.LFE26-.LFB26
	.align 8
.LEFDE12:
.LSFDE14:
	.long	.LEFDE14-.LASFDE14
.LASFDE14:
	.long	.Lframe0
	.quad	.LFB27
	.quad	.LFE27-.LFB27
	.align 8
.LEFDE14:
.LSFDE16:
	.long	.LEFDE16-.LASFDE16
.LASFDE16:
	.long	.Lframe0
	.quad	.LFB28
	.quad	.LFE28-.LFB28
	.align 8
.LEFDE16:
.LSFDE18:
	.long	.LEFDE18-.LASFDE18
.LASFDE18:
	.long	.Lframe0
	.quad	.LFB29
	.quad	.LFE29-.LFB29
	.align 8
.LEFDE18:
.LSFDE20:
	.long	.LEFDE20-.LASFDE20
.LASFDE20:
	.long	.Lframe0
	.quad	.LFB30
	.quad	.LFE30-.LFB30
	.align 8
.LEFDE20:
.LSFDE22:
	.long	.LEFDE22-.LASFDE22
.LASFDE22:
	.long	.Lframe0
	.quad	.LFB31
	.quad	.LFE31-.LFB31
	.byte	0x4
	.long	.LCFI0-.LFB31
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE22:
.LSFDE24:
	.long	.LEFDE24-.LASFDE24
.LASFDE24:
	.long	.Lframe0
	.quad	.LFB32
	.quad	.LFE32-.LFB32
	.align 8
.LEFDE24:
.LSFDE26:
	.long	.LEFDE26-.LASFDE26
.LASFDE26:
	.long	.Lframe0
	.quad	.LFB33
	.quad	.LFE33-.LFB33
	.byte	0x4
	.long	.LCFI1-.LFB33
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE26:
.LSFDE28:
	.long	.LEFDE28-.LASFDE28
.LASFDE28:
	.long	.Lframe0
	.quad	.LFB34
	.quad	.LFE34-.LFB34
	.align 8
.LEFDE28:
.LSFDE30:
	.long	.LEFDE30-.LASFDE30
.LASFDE30:
	.long	.Lframe0
	.quad	.LFB35
	.quad	.LFE35-.LFB35
	.byte	0x4
	.long	.LCFI2-.LFB35
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE30:
.LSFDE32:
	.long	.LEFDE32-.LASFDE32
.LASFDE32:
	.long	.Lframe0
	.quad	.LFB36
	.quad	.LFE36-.LFB36
	.byte	0x4
	.long	.LCFI3-.LFB36
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI8-.LCFI7
	.byte	0xe
	.uleb128 0x40
	.byte	0x83
	.uleb128 0x6
	.byte	0x86
	.uleb128 0x5
	.byte	0x8c
	.uleb128 0x4
	.byte	0x8d
	.uleb128 0x3
	.byte	0x8e
	.uleb128 0x2
	.align 8
.LEFDE32:
.LSFDE34:
	.long	.LEFDE34-.LASFDE34
.LASFDE34:
	.long	.Lframe0
	.quad	.LFB19
	.quad	.LFE19-.LFB19
	.byte	0x4
	.long	.LCFI9-.LFB19
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI10-.LCFI9
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI11-.LCFI10
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI12-.LCFI11
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI13-.LCFI12
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI14-.LCFI13
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI15-.LCFI14
	.byte	0xe
	.uleb128 0x60
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
.LEFDE34:
.LSFDE36:
	.long	.LEFDE36-.LASFDE36
.LASFDE36:
	.long	.Lframe0
	.quad	.LFB37
	.quad	.LFE37-.LFB37
	.byte	0x4
	.long	.LCFI16-.LFB37
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE36:
.LSFDE38:
	.long	.LEFDE38-.LASFDE38
.LASFDE38:
	.long	.Lframe0
	.quad	.LFB38
	.quad	.LFE38-.LFB38
	.byte	0x4
	.long	.LCFI17-.LFB38
	.byte	0xe
	.uleb128 0x10
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
	.long	.LFB20
	.long	.LFE20-.LFB20
	.uleb128 0x0
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB21
	.long	.LFE21-.LFB21
	.uleb128 0x0
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB22
	.long	.LFE22-.LFB22
	.uleb128 0x0
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB23
	.long	.LFE23-.LFB23
	.uleb128 0x0
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB24
	.long	.LFE24-.LFB24
	.uleb128 0x0
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB25
	.long	.LFE25-.LFB25
	.uleb128 0x0
	.align 8
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.long	.LFB26
	.long	.LFE26-.LFB26
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
	.align 8
.LEFDE15:
.LSFDE17:
	.long	.LEFDE17-.LASFDE17
.LASFDE17:
	.long	.LASFDE17-.Lframe1
	.long	.LFB28
	.long	.LFE28-.LFB28
	.uleb128 0x0
	.align 8
.LEFDE17:
.LSFDE19:
	.long	.LEFDE19-.LASFDE19
.LASFDE19:
	.long	.LASFDE19-.Lframe1
	.long	.LFB29
	.long	.LFE29-.LFB29
	.uleb128 0x0
	.align 8
.LEFDE19:
.LSFDE21:
	.long	.LEFDE21-.LASFDE21
.LASFDE21:
	.long	.LASFDE21-.Lframe1
	.long	.LFB30
	.long	.LFE30-.LFB30
	.uleb128 0x0
	.align 8
.LEFDE21:
.LSFDE23:
	.long	.LEFDE23-.LASFDE23
.LASFDE23:
	.long	.LASFDE23-.Lframe1
	.long	.LFB31
	.long	.LFE31-.LFB31
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB31
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE23:
.LSFDE25:
	.long	.LEFDE25-.LASFDE25
.LASFDE25:
	.long	.LASFDE25-.Lframe1
	.long	.LFB32
	.long	.LFE32-.LFB32
	.uleb128 0x0
	.align 8
.LEFDE25:
.LSFDE27:
	.long	.LEFDE27-.LASFDE27
.LASFDE27:
	.long	.LASFDE27-.Lframe1
	.long	.LFB33
	.long	.LFE33-.LFB33
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI1-.LFB33
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE27:
.LSFDE29:
	.long	.LEFDE29-.LASFDE29
.LASFDE29:
	.long	.LASFDE29-.Lframe1
	.long	.LFB34
	.long	.LFE34-.LFB34
	.uleb128 0x0
	.align 8
.LEFDE29:
.LSFDE31:
	.long	.LEFDE31-.LASFDE31
.LASFDE31:
	.long	.LASFDE31-.Lframe1
	.long	.LFB35
	.long	.LFE35-.LFB35
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI2-.LFB35
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE31:
.LSFDE33:
	.long	.LEFDE33-.LASFDE33
.LASFDE33:
	.long	.LASFDE33-.Lframe1
	.long	.LFB36
	.long	.LFE36-.LFB36
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI3-.LFB36
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI8-.LCFI7
	.byte	0xe
	.uleb128 0x40
	.byte	0x83
	.uleb128 0x6
	.byte	0x86
	.uleb128 0x5
	.byte	0x8c
	.uleb128 0x4
	.byte	0x8d
	.uleb128 0x3
	.byte	0x8e
	.uleb128 0x2
	.align 8
.LEFDE33:
.LSFDE35:
	.long	.LEFDE35-.LASFDE35
.LASFDE35:
	.long	.LASFDE35-.Lframe1
	.long	.LFB19
	.long	.LFE19-.LFB19
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI9-.LFB19
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI10-.LCFI9
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI11-.LCFI10
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI12-.LCFI11
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI13-.LCFI12
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI14-.LCFI13
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI15-.LCFI14
	.byte	0xe
	.uleb128 0x60
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
.LEFDE35:
.LSFDE37:
	.long	.LEFDE37-.LASFDE37
.LASFDE37:
	.long	.LASFDE37-.Lframe1
	.long	.LFB37
	.long	.LFE37-.LFB37
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI16-.LFB37
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE37:
.LSFDE39:
	.long	.LEFDE39-.LASFDE39
.LASFDE39:
	.long	.LASFDE39-.Lframe1
	.long	.LFB38
	.long	.LFE38-.LFB38
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI17-.LFB38
	.byte	0xe
	.uleb128 0x10
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
	.byte	0x61
	.quad	.LVL3-.Ltext0
	.quad	.LVL5-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	0x0
	.quad	0x0
.LLST3:
	.quad	.LVL6-.Ltext0
	.quad	.LVL8-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST5:
	.quad	.LVL10-.Ltext0
	.quad	.LVL11-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL12-.Ltext0
	.quad	.LVL13-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	0x0
	.quad	0x0
.LLST7:
	.quad	.LVL14-.Ltext0
	.quad	.LVL16-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST9:
	.quad	.LVL18-.Ltext0
	.quad	.LVL24-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL25-.Ltext0
	.quad	.LVL31-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL31-.Ltext0
	.quad	.LVL32-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL32-.Ltext0
	.quad	.LFE24-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	0x0
	.quad	0x0
.LLST10:
	.quad	.LVL19-.Ltext0
	.quad	.LVL24-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL26-.Ltext0
	.quad	.LVL33-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	0x0
	.quad	0x0
.LLST11:
	.quad	.LVL20-.Ltext0
	.quad	.LVL24-.Ltext0
	.value	0x1
	.byte	0x65
	.quad	.LVL27-.Ltext0
	.quad	.LFE24-.Ltext0
	.value	0x1
	.byte	0x65
	.quad	0x0
	.quad	0x0
.LLST12:
	.quad	.LVL21-.Ltext0
	.quad	.LVL24-.Ltext0
	.value	0x1
	.byte	0x64
	.quad	.LVL28-.Ltext0
	.quad	.LFE24-.Ltext0
	.value	0x1
	.byte	0x64
	.quad	0x0
	.quad	0x0
.LLST13:
	.quad	.LVL22-.Ltext0
	.quad	.LVL24-.Ltext0
	.value	0x1
	.byte	0x63
	.quad	.LVL29-.Ltext0
	.quad	.LFE24-.Ltext0
	.value	0x1
	.byte	0x63
	.quad	0x0
	.quad	0x0
.LLST14:
	.quad	.LVL23-.Ltext0
	.quad	.LVL24-.Ltext0
	.value	0x1
	.byte	0x62
	.quad	.LVL30-.Ltext0
	.quad	.LFE24-.Ltext0
	.value	0x1
	.byte	0x62
	.quad	0x0
	.quad	0x0
.LLST16:
	.quad	.LVL34-.Ltext0
	.quad	.LVL36-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST18:
	.quad	.LVL38-.Ltext0
	.quad	.LVL40-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL41-.Ltext0
	.quad	.LVL45-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL45-.Ltext0
	.quad	.LVL46-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL46-.Ltext0
	.quad	.LFE26-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	0x0
	.quad	0x0
.LLST19:
	.quad	.LVL39-.Ltext0
	.quad	.LVL40-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL42-.Ltext0
	.quad	.LVL43-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL44-.Ltext0
	.quad	.LVL47-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	0x0
	.quad	0x0
.LLST21:
	.quad	.LVL48-.Ltext0
	.quad	.LVL50-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST23:
	.quad	.LVL52-.Ltext0
	.quad	.LVL54-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL55-.Ltext0
	.quad	.LVL57-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL57-.Ltext0
	.quad	.LVL58-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL58-.Ltext0
	.quad	.LFE28-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	0x0
	.quad	0x0
.LLST24:
	.quad	.LVL53-.Ltext0
	.quad	.LVL54-.Ltext0
	.value	0x1
	.byte	0x63
	.quad	.LVL56-.Ltext0
	.quad	.LFE28-.Ltext0
	.value	0x1
	.byte	0x63
	.quad	0x0
	.quad	0x0
.LLST26:
	.quad	.LVL59-.Ltext0
	.quad	.LVL61-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST28:
	.quad	.LVL62-.Ltext0
	.quad	.LVL71-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL73-.Ltext0
	.quad	.LFE30-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST29:
	.quad	.LVL62-.Ltext0
	.quad	.LVL64-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL64-.Ltext0
	.quad	.LVL65-.Ltext0
	.value	0x1
	.byte	0x58
	.quad	.LVL65-.Ltext0
	.quad	.LVL66-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL66-.Ltext0
	.quad	.LVL68-.Ltext0
	.value	0x1
	.byte	0x58
	.quad	.LVL68-.Ltext0
	.quad	.LVL69-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL69-.Ltext0
	.quad	.LFE30-.Ltext0
	.value	0x1
	.byte	0x58
	.quad	0x0
	.quad	0x0
.LLST30:
	.quad	.LVL67-.Ltext0
	.quad	.LVL68-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL70-.Ltext0
	.quad	.LVL77-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	0x0
	.quad	0x0
.LLST31:
	.quad	.LVL66-.Ltext0
	.quad	.LVL68-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL69-.Ltext0
	.quad	.LVL72-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL72-.Ltext0
	.quad	.LVL73-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	.LVL73-.Ltext0
	.quad	.LVL74-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL74-.Ltext0
	.quad	.LFE30-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	0x0
	.quad	0x0
.LLST32:
	.quad	.LFB31-.Ltext0
	.quad	.LCFI0-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI0-.Ltext0
	.quad	.LFE31-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST33:
	.quad	.LVL78-.Ltext0
	.quad	.LVL80-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST35:
	.quad	.LVL81-.Ltext0
	.quad	.LVL99-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL101-.Ltext0
	.quad	.LFE32-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST36:
	.quad	.LVL81-.Ltext0
	.quad	.LVL83-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL83-.Ltext0
	.quad	.LVL84-.Ltext0
	.value	0x1
	.byte	0x58
	.quad	.LVL84-.Ltext0
	.quad	.LVL85-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL85-.Ltext0
	.quad	.LVL87-.Ltext0
	.value	0x1
	.byte	0x58
	.quad	.LVL87-.Ltext0
	.quad	.LVL88-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL88-.Ltext0
	.quad	.LFE32-.Ltext0
	.value	0x1
	.byte	0x58
	.quad	0x0
	.quad	0x0
.LLST37:
	.quad	.LVL90-.Ltext0
	.quad	.LVL94-.Ltext0
	.value	0x1
	.byte	0x65
	.quad	.LVL95-.Ltext0
	.quad	.LFE32-.Ltext0
	.value	0x1
	.byte	0x65
	.quad	0x0
	.quad	0x0
.LLST38:
	.quad	.LVL91-.Ltext0
	.quad	.LVL94-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL96-.Ltext0
	.quad	.LVL104-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	0x0
	.quad	0x0
.LLST39:
	.quad	.LVL92-.Ltext0
	.quad	.LVL94-.Ltext0
	.value	0x1
	.byte	0x64
	.quad	.LVL97-.Ltext0
	.quad	.LFE32-.Ltext0
	.value	0x1
	.byte	0x64
	.quad	0x0
	.quad	0x0
.LLST40:
	.quad	.LVL93-.Ltext0
	.quad	.LVL94-.Ltext0
	.value	0x1
	.byte	0x62
	.quad	.LVL98-.Ltext0
	.quad	.LVL105-.Ltext0
	.value	0x1
	.byte	0x62
	.quad	0x0
	.quad	0x0
.LLST41:
	.quad	.LVL86-.Ltext0
	.quad	.LVL87-.Ltext0
	.value	0x1
	.byte	0x66
	.quad	.LVL89-.Ltext0
	.quad	.LVL106-.Ltext0
	.value	0x1
	.byte	0x66
	.quad	.LVL106-.Ltext0
	.quad	.LVL107-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	0x0
	.quad	0x0
.LLST42:
	.quad	.LVL85-.Ltext0
	.quad	.LVL87-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL88-.Ltext0
	.quad	.LVL100-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL100-.Ltext0
	.quad	.LVL101-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	.LVL101-.Ltext0
	.quad	.LVL102-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL102-.Ltext0
	.quad	.LFE32-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	0x0
	.quad	0x0
.LLST43:
	.quad	.LFB33-.Ltext0
	.quad	.LCFI1-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI1-.Ltext0
	.quad	.LFE33-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST44:
	.quad	.LVL108-.Ltext0
	.quad	.LVL110-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST46:
	.quad	.LVL111-.Ltext0
	.quad	.LVL135-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL137-.Ltext0
	.quad	.LFE34-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST47:
	.quad	.LVL111-.Ltext0
	.quad	.LVL113-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL113-.Ltext0
	.quad	.LVL114-.Ltext0
	.value	0x1
	.byte	0x58
	.quad	.LVL114-.Ltext0
	.quad	.LVL115-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL115-.Ltext0
	.quad	.LVL117-.Ltext0
	.value	0x1
	.byte	0x58
	.quad	.LVL117-.Ltext0
	.quad	.LVL118-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL118-.Ltext0
	.quad	.LFE34-.Ltext0
	.value	0x1
	.byte	0x58
	.quad	0x0
	.quad	0x0
.LLST48:
	.quad	.LVL120-.Ltext0
	.quad	.LVL127-.Ltext0
	.value	0x1
	.byte	0x68
	.quad	.LVL128-.Ltext0
	.quad	.LFE34-.Ltext0
	.value	0x1
	.byte	0x68
	.quad	0x0
	.quad	0x0
.LLST49:
	.quad	.LVL121-.Ltext0
	.quad	.LVL127-.Ltext0
	.value	0x1
	.byte	0x63
	.quad	.LVL129-.Ltext0
	.quad	.LVL140-.Ltext0
	.value	0x1
	.byte	0x63
	.quad	0x0
	.quad	0x0
.LLST50:
	.quad	.LVL122-.Ltext0
	.quad	.LVL127-.Ltext0
	.value	0x1
	.byte	0x67
	.quad	.LVL130-.Ltext0
	.quad	.LFE34-.Ltext0
	.value	0x1
	.byte	0x67
	.quad	0x0
	.quad	0x0
.LLST51:
	.quad	.LVL123-.Ltext0
	.quad	.LVL127-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL131-.Ltext0
	.quad	.LVL141-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	0x0
	.quad	0x0
.LLST52:
	.quad	.LVL124-.Ltext0
	.quad	.LVL127-.Ltext0
	.value	0x1
	.byte	0x66
	.quad	.LVL132-.Ltext0
	.quad	.LFE34-.Ltext0
	.value	0x1
	.byte	0x66
	.quad	0x0
	.quad	0x0
.LLST53:
	.quad	.LVL125-.Ltext0
	.quad	.LVL127-.Ltext0
	.value	0x1
	.byte	0x65
	.quad	.LVL133-.Ltext0
	.quad	.LFE34-.Ltext0
	.value	0x1
	.byte	0x65
	.quad	0x0
	.quad	0x0
.LLST54:
	.quad	.LVL126-.Ltext0
	.quad	.LVL127-.Ltext0
	.value	0x1
	.byte	0x64
	.quad	.LVL134-.Ltext0
	.quad	.LFE34-.Ltext0
	.value	0x1
	.byte	0x64
	.quad	0x0
	.quad	0x0
.LLST55:
	.quad	.LVL112-.Ltext0
	.quad	.LVL142-.Ltext0
	.value	0x1
	.byte	0x62
	.quad	0x0
	.quad	0x0
.LLST56:
	.quad	.LVL116-.Ltext0
	.quad	.LVL117-.Ltext0
	.value	0x1
	.byte	0x69
	.quad	.LVL119-.Ltext0
	.quad	.LVL143-.Ltext0
	.value	0x1
	.byte	0x69
	.quad	.LVL143-.Ltext0
	.quad	.LVL144-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	0x0
	.quad	0x0
.LLST57:
	.quad	.LVL115-.Ltext0
	.quad	.LVL117-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL118-.Ltext0
	.quad	.LVL136-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL136-.Ltext0
	.quad	.LVL137-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	.LVL137-.Ltext0
	.quad	.LVL138-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL138-.Ltext0
	.quad	.LFE34-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	0x0
	.quad	0x0
.LLST58:
	.quad	.LFB35-.Ltext0
	.quad	.LCFI2-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI2-.Ltext0
	.quad	.LFE35-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST59:
	.quad	.LVL145-.Ltext0
	.quad	.LVL147-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST60:
	.quad	.LFB36-.Ltext0
	.quad	.LCFI3-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI3-.Ltext0
	.quad	.LCFI4-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI4-.Ltext0
	.quad	.LCFI5-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 24
	.quad	.LCFI5-.Ltext0
	.quad	.LCFI6-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 32
	.quad	.LCFI6-.Ltext0
	.quad	.LCFI7-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 40
	.quad	.LCFI7-.Ltext0
	.quad	.LCFI8-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 48
	.quad	.LCFI8-.Ltext0
	.quad	.LFE36-.Ltext0
	.value	0x3
	.byte	0x77
	.sleb128 64
	.quad	0x0
	.quad	0x0
.LLST61:
	.quad	.LVL148-.Ltext0
	.quad	.LVL149-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL149-.Ltext0
	.quad	.LVL153-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	0x0
	.quad	0x0
.LLST62:
	.quad	.LVL148-.Ltext0
	.quad	.LVL149-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL149-.Ltext0
	.quad	.LVL154-.Ltext0
	.value	0x1
	.byte	0x5d
	.quad	0x0
	.quad	0x0
.LLST63:
	.quad	.LVL148-.Ltext0
	.quad	.LVL149-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL149-.Ltext0
	.quad	.LVL155-.Ltext0
	.value	0x1
	.byte	0x5e
	.quad	0x0
	.quad	0x0
.LLST64:
	.quad	.LVL149-.Ltext0
	.quad	.LVL152-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	0x0
	.quad	0x0
.LLST65:
	.quad	.LFB19-.Ltext0
	.quad	.LCFI9-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI9-.Ltext0
	.quad	.LCFI10-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI10-.Ltext0
	.quad	.LCFI11-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 24
	.quad	.LCFI11-.Ltext0
	.quad	.LCFI12-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 32
	.quad	.LCFI12-.Ltext0
	.quad	.LCFI13-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 40
	.quad	.LCFI13-.Ltext0
	.quad	.LCFI14-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 48
	.quad	.LCFI14-.Ltext0
	.quad	.LCFI15-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 56
	.quad	.LCFI15-.Ltext0
	.quad	.LFE19-.Ltext0
	.value	0x3
	.byte	0x77
	.sleb128 96
	.quad	0x0
	.quad	0x0
.LLST66:
	.quad	.LVL156-.Ltext0
	.quad	.LVL157-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL157-.Ltext0
	.quad	.LVL159-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 36
	.quad	.LVL159-.Ltext0
	.quad	.LFE19-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 -60
	.quad	0x0
	.quad	0x0
.LLST67:
	.quad	.LVL158-.Ltext0
	.quad	.LVL162-.Ltext0
	.value	0x1
	.byte	0x5d
	.quad	0x0
	.quad	0x0
.LLST68:
	.quad	.LVL160-.Ltext0
	.quad	.LVL161-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	0x0
	.quad	0x0
.LLST69:
	.quad	.LFB37-.Ltext0
	.quad	.LCFI16-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI16-.Ltext0
	.quad	.LFE37-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST70:
	.quad	.LVL163-.Ltext0
	.quad	.LVL164-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL164-.Ltext0
	.quad	.LVL165-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	.LVL166-.Ltext0
	.quad	.LVL167-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	0x0
	.quad	0x0
.LLST71:
	.quad	.LFB38-.Ltext0
	.quad	.LCFI17-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI17-.Ltext0
	.quad	.LFE38-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST72:
	.quad	.LVL168-.Ltext0
	.quad	.LVL170-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL171-.Ltext0
	.quad	.LVL172-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST73:
	.quad	.LVL168-.Ltext0
	.quad	.LVL171-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL171-.Ltext0
	.quad	.LVL173-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	0x0
	.quad	0x0
.LLST74:
	.quad	.LVL169-.Ltext0
	.quad	.LVL171-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL171-.Ltext0
	.quad	.LVL173-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	0x0
	.quad	0x0
	.section	.debug_info
	.long	0xbf2
	.value	0x2
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF84
	.byte	0x1
	.long	.LASF85
	.long	.LASF86
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
	.uleb128 0x7
	.long	0x6e
	.long	0x90
	.uleb128 0x8
	.long	0x65
	.byte	0x37
	.byte	0x0
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF8
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF9
	.uleb128 0x9
	.long	.LASF12
	.byte	0x1
	.byte	0x9
	.long	0xa9
	.uleb128 0x5
	.byte	0x8
	.long	0xaf
	.uleb128 0xa
	.string	"ELE"
	.byte	0x10
	.byte	0x1
	.byte	0x9
	.long	0xd8
	.uleb128 0xb
	.string	"val"
	.byte	0x1
	.byte	0xc
	.long	0xd8
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0xc
	.long	.LASF10
	.byte	0x1
	.byte	0xd
	.long	0x9e
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.byte	0x0
	.uleb128 0x2
	.byte	0x8
	.byte	0x4
	.long	.LASF11
	.uleb128 0x9
	.long	.LASF13
	.byte	0x1
	.byte	0xe
	.long	0xaf
	.uleb128 0x9
	.long	.LASF14
	.byte	0x1
	.byte	0x90
	.long	0xf5
	.uleb128 0xd
	.byte	0x1
	.long	0xd8
	.long	0x106
	.uleb128 0x8
	.long	0x65
	.byte	0x1
	.byte	0x0
	.uleb128 0xe
	.byte	0x10
	.byte	0x1
	.byte	0x91
	.long	0x121
	.uleb128 0xf
	.string	"v"
	.byte	0x1
	.byte	0x92
	.long	0xea
	.uleb128 0xf
	.string	"d"
	.byte	0x1
	.byte	0x93
	.long	0x121
	.byte	0x0
	.uleb128 0x7
	.long	0xd8
	.long	0x131
	.uleb128 0x8
	.long	0x65
	.byte	0x1
	.byte	0x0
	.uleb128 0x9
	.long	.LASF15
	.byte	0x1
	.byte	0x94
	.long	0x106
	.uleb128 0x10
	.long	.LASF16
	.byte	0x1
	.value	0x12e
	.long	0x148
	.uleb128 0x5
	.byte	0x8
	.long	0x14e
	.uleb128 0x11
	.byte	0x1
	.long	0xd8
	.long	0x15e
	.uleb128 0x12
	.long	0x57
	.byte	0x0
	.uleb128 0x13
	.byte	0x18
	.byte	0x1
	.value	0x144
	.long	0x195
	.uleb128 0x14
	.string	"fun"
	.byte	0x1
	.value	0x145
	.long	0x13c
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0x15
	.long	.LASF17
	.byte	0x1
	.value	0x146
	.long	0x68
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x15
	.long	.LASF18
	.byte	0x1
	.value	0x147
	.long	0x68
	.byte	0x2
	.byte	0x23
	.uleb128 0x10
	.byte	0x0
	.uleb128 0x10
	.long	.LASF19
	.byte	0x1
	.value	0x148
	.long	0x15e
	.uleb128 0x16
	.byte	0x1
	.long	.LASF87
	.byte	0x2
	.byte	0x68
	.byte	0x1
	.long	0x57
	.byte	0x2
	.long	0x1c0
	.uleb128 0x17
	.long	.LASF88
	.byte	0x2
	.byte	0x67
	.long	0x75
	.uleb128 0x18
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.long	.LASF20
	.byte	0x1
	.byte	0x34
	.byte	0x1
	.long	0xd8
	.quad	.LFB20
	.quad	.LFE20
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x200
	.uleb128 0x1a
	.string	"ls"
	.byte	0x1
	.byte	0x33
	.long	0x9e
	.byte	0x1
	.byte	0x55
	.uleb128 0x1b
	.string	"val"
	.byte	0x1
	.byte	0x35
	.long	0xd8
	.long	.LLST1
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.long	.LASF21
	.byte	0x1
	.byte	0x3e
	.byte	0x1
	.long	0xd8
	.quad	.LFB21
	.quad	.LFE21
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x232
	.uleb128 0x1c
	.string	"t"
	.byte	0x1
	.byte	0x3d
	.long	0x57
	.long	.LLST3
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.long	.LASF22
	.byte	0x1
	.byte	0x43
	.byte	0x1
	.long	0xd8
	.quad	.LFB22
	.quad	.LFE22
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x289
	.uleb128 0x1d
	.long	.LASF23
	.byte	0x1
	.byte	0x43
	.long	0x289
	.byte	0x1
	.byte	0x55
	.uleb128 0x1a
	.string	"cnt"
	.byte	0x1
	.byte	0x43
	.long	0x57
	.byte	0x1
	.byte	0x54
	.uleb128 0x1e
	.string	"i"
	.byte	0x1
	.byte	0x44
	.long	0x57
	.uleb128 0x1b
	.string	"val"
	.byte	0x1
	.byte	0x45
	.long	0xd8
	.long	.LLST5
	.byte	0x0
	.uleb128 0x5
	.byte	0x8
	.long	0xd8
	.uleb128 0x19
	.byte	0x1
	.long	.LASF24
	.byte	0x1
	.byte	0x4d
	.byte	0x1
	.long	0xd8
	.quad	.LFB23
	.quad	.LFE23
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x2c1
	.uleb128 0x1c
	.string	"t"
	.byte	0x1
	.byte	0x4c
	.long	0x57
	.long	.LLST7
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.long	.LASF25
	.byte	0x1
	.byte	0x53
	.byte	0x1
	.long	0xd8
	.quad	.LFB24
	.quad	.LFE24
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x358
	.uleb128 0x1d
	.long	.LASF23
	.byte	0x1
	.byte	0x53
	.long	0x289
	.byte	0x1
	.byte	0x55
	.uleb128 0x1a
	.string	"cnt"
	.byte	0x1
	.byte	0x53
	.long	0x57
	.byte	0x1
	.byte	0x54
	.uleb128 0x1b
	.string	"i"
	.byte	0x1
	.byte	0x54
	.long	0x57
	.long	.LLST9
	.uleb128 0x1f
	.long	.LASF26
	.byte	0x1
	.byte	0x55
	.long	0xd8
	.long	.LLST10
	.uleb128 0x1f
	.long	.LASF27
	.byte	0x1
	.byte	0x56
	.long	0xd8
	.long	.LLST11
	.uleb128 0x1f
	.long	.LASF28
	.byte	0x1
	.byte	0x57
	.long	0xd8
	.long	.LLST12
	.uleb128 0x1f
	.long	.LASF29
	.byte	0x1
	.byte	0x58
	.long	0xd8
	.long	.LLST13
	.uleb128 0x1f
	.long	.LASF30
	.byte	0x1
	.byte	0x59
	.long	0xd8
	.long	.LLST14
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.long	.LASF31
	.byte	0x1
	.byte	0x67
	.byte	0x1
	.long	0xd8
	.quad	.LFB25
	.quad	.LFE25
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x38a
	.uleb128 0x1c
	.string	"t"
	.byte	0x1
	.byte	0x66
	.long	0x57
	.long	.LLST16
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.long	.LASF32
	.byte	0x1
	.byte	0x6c
	.byte	0x1
	.long	0xd8
	.quad	.LFB26
	.quad	.LFE26
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x3e5
	.uleb128 0x1d
	.long	.LASF23
	.byte	0x1
	.byte	0x6c
	.long	0x289
	.byte	0x1
	.byte	0x55
	.uleb128 0x1a
	.string	"cnt"
	.byte	0x1
	.byte	0x6c
	.long	0x57
	.byte	0x1
	.byte	0x54
	.uleb128 0x1b
	.string	"i"
	.byte	0x1
	.byte	0x6d
	.long	0x57
	.long	.LLST18
	.uleb128 0x1b
	.string	"val"
	.byte	0x1
	.byte	0x6e
	.long	0xd8
	.long	.LLST19
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.long	.LASF33
	.byte	0x1
	.byte	0x78
	.byte	0x1
	.long	0xd8
	.quad	.LFB27
	.quad	.LFE27
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x417
	.uleb128 0x1c
	.string	"t"
	.byte	0x1
	.byte	0x77
	.long	0x57
	.long	.LLST21
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.long	.LASF34
	.byte	0x1
	.byte	0x7e
	.byte	0x1
	.long	0xd8
	.quad	.LFB28
	.quad	.LFE28
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x472
	.uleb128 0x1d
	.long	.LASF23
	.byte	0x1
	.byte	0x7e
	.long	0x289
	.byte	0x1
	.byte	0x55
	.uleb128 0x1a
	.string	"cnt"
	.byte	0x1
	.byte	0x7e
	.long	0x57
	.byte	0x1
	.byte	0x54
	.uleb128 0x1b
	.string	"i"
	.byte	0x1
	.byte	0x7f
	.long	0x57
	.long	.LLST23
	.uleb128 0x1b
	.string	"val"
	.byte	0x1
	.byte	0x80
	.long	0xd8
	.long	.LLST24
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.long	.LASF35
	.byte	0x1
	.byte	0x8a
	.byte	0x1
	.long	0xd8
	.quad	.LFB29
	.quad	.LFE29
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x4a4
	.uleb128 0x1c
	.string	"t"
	.byte	0x1
	.byte	0x89
	.long	0x57
	.long	.LLST26
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.long	.LASF36
	.byte	0x1
	.byte	0x9c
	.byte	0x1
	.long	0xd8
	.quad	.LFB30
	.quad	.LFE30
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x53a
	.uleb128 0x20
	.long	.LASF23
	.byte	0x1
	.byte	0x9b
	.long	0x289
	.long	.LLST28
	.uleb128 0x1c
	.string	"cnt"
	.byte	0x1
	.byte	0x9b
	.long	0x57
	.long	.LLST29
	.uleb128 0x21
	.long	.LASF37
	.byte	0x1
	.byte	0x9d
	.long	0x131
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x21
	.long	.LASF38
	.byte	0x1
	.byte	0x9e
	.long	0xea
	.byte	0x1
	.byte	0x62
	.uleb128 0x1f
	.long	.LASF39
	.byte	0x1
	.byte	0x9f
	.long	0xd8
	.long	.LLST30
	.uleb128 0x1f
	.long	.LASF40
	.byte	0x1
	.byte	0xa0
	.long	0x289
	.long	.LLST31
	.uleb128 0x1e
	.string	"i"
	.byte	0x1
	.byte	0xa2
	.long	0x57
	.uleb128 0x22
	.long	.Ldebug_ranges0+0x0
	.uleb128 0x23
	.long	.LASF41
	.byte	0x1
	.byte	0xae
	.long	0xea
	.byte	0x0
	.byte	0x0
	.uleb128 0x24
	.byte	0x1
	.long	.LASF42
	.byte	0x1
	.byte	0xbf
	.byte	0x1
	.long	0xd8
	.quad	.LFB31
	.quad	.LFE31
	.long	.LLST32
	.long	0x56d
	.uleb128 0x1c
	.string	"t"
	.byte	0x1
	.byte	0xbe
	.long	0x57
	.long	.LLST33
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.long	.LASF43
	.byte	0x1
	.byte	0xc7
	.byte	0x1
	.long	0xd8
	.quad	.LFB32
	.quad	.LFE32
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x66b
	.uleb128 0x20
	.long	.LASF23
	.byte	0x1
	.byte	0xc6
	.long	0x289
	.long	.LLST35
	.uleb128 0x1c
	.string	"cnt"
	.byte	0x1
	.byte	0xc6
	.long	0x57
	.long	.LLST36
	.uleb128 0x21
	.long	.LASF37
	.byte	0x1
	.byte	0xc8
	.long	0x131
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x1f
	.long	.LASF44
	.byte	0x1
	.byte	0xc9
	.long	0xea
	.long	.LLST37
	.uleb128 0x1f
	.long	.LASF45
	.byte	0x1
	.byte	0xc9
	.long	0xea
	.long	.LLST38
	.uleb128 0x1f
	.long	.LASF46
	.byte	0x1
	.byte	0xc9
	.long	0xea
	.long	.LLST39
	.uleb128 0x1f
	.long	.LASF47
	.byte	0x1
	.byte	0xc9
	.long	0xea
	.long	.LLST40
	.uleb128 0x21
	.long	.LASF48
	.byte	0x1
	.byte	0xc9
	.long	0xea
	.byte	0x1
	.byte	0x63
	.uleb128 0x1f
	.long	.LASF39
	.byte	0x1
	.byte	0xca
	.long	0xd8
	.long	.LLST41
	.uleb128 0x1f
	.long	.LASF40
	.byte	0x1
	.byte	0xcb
	.long	0x289
	.long	.LLST42
	.uleb128 0x1e
	.string	"i"
	.byte	0x1
	.byte	0xcd
	.long	0x57
	.uleb128 0x22
	.long	.Ldebug_ranges0+0x30
	.uleb128 0x23
	.long	.LASF49
	.byte	0x1
	.byte	0xd9
	.long	0xea
	.uleb128 0x23
	.long	.LASF50
	.byte	0x1
	.byte	0xdb
	.long	0xea
	.uleb128 0x23
	.long	.LASF51
	.byte	0x1
	.byte	0xdd
	.long	0xea
	.uleb128 0x23
	.long	.LASF52
	.byte	0x1
	.byte	0xdf
	.long	0xea
	.uleb128 0x23
	.long	.LASF53
	.byte	0x1
	.byte	0xe1
	.long	0xea
	.byte	0x0
	.byte	0x0
	.uleb128 0x24
	.byte	0x1
	.long	.LASF54
	.byte	0x1
	.byte	0xf2
	.byte	0x1
	.long	0xd8
	.quad	.LFB33
	.quad	.LFE33
	.long	.LLST43
	.long	0x69e
	.uleb128 0x1c
	.string	"t"
	.byte	0x1
	.byte	0xf1
	.long	0x57
	.long	.LLST44
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.long	.LASF55
	.byte	0x1
	.byte	0xf9
	.byte	0x1
	.long	0xd8
	.quad	.LFB34
	.quad	.LFE34
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x7f4
	.uleb128 0x20
	.long	.LASF23
	.byte	0x1
	.byte	0xf8
	.long	0x289
	.long	.LLST46
	.uleb128 0x1c
	.string	"cnt"
	.byte	0x1
	.byte	0xf8
	.long	0x57
	.long	.LLST47
	.uleb128 0x21
	.long	.LASF37
	.byte	0x1
	.byte	0xfa
	.long	0x131
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x1f
	.long	.LASF44
	.byte	0x1
	.byte	0xfb
	.long	0xea
	.long	.LLST48
	.uleb128 0x1f
	.long	.LASF45
	.byte	0x1
	.byte	0xfb
	.long	0xea
	.long	.LLST49
	.uleb128 0x1f
	.long	.LASF46
	.byte	0x1
	.byte	0xfb
	.long	0xea
	.long	.LLST50
	.uleb128 0x1f
	.long	.LASF47
	.byte	0x1
	.byte	0xfb
	.long	0xea
	.long	.LLST51
	.uleb128 0x1f
	.long	.LASF48
	.byte	0x1
	.byte	0xfb
	.long	0xea
	.long	.LLST52
	.uleb128 0x1f
	.long	.LASF56
	.byte	0x1
	.byte	0xfb
	.long	0xea
	.long	.LLST53
	.uleb128 0x1f
	.long	.LASF57
	.byte	0x1
	.byte	0xfb
	.long	0xea
	.long	.LLST54
	.uleb128 0x1f
	.long	.LASF58
	.byte	0x1
	.byte	0xfb
	.long	0xea
	.long	.LLST55
	.uleb128 0x1f
	.long	.LASF39
	.byte	0x1
	.byte	0xfc
	.long	0xd8
	.long	.LLST56
	.uleb128 0x1f
	.long	.LASF40
	.byte	0x1
	.byte	0xfd
	.long	0x289
	.long	.LLST57
	.uleb128 0x1e
	.string	"i"
	.byte	0x1
	.byte	0xff
	.long	0x57
	.uleb128 0x22
	.long	.Ldebug_ranges0+0x60
	.uleb128 0x25
	.long	.LASF49
	.byte	0x1
	.value	0x10b
	.long	0xea
	.uleb128 0x25
	.long	.LASF50
	.byte	0x1
	.value	0x10d
	.long	0xea
	.uleb128 0x25
	.long	.LASF51
	.byte	0x1
	.value	0x10f
	.long	0xea
	.uleb128 0x25
	.long	.LASF52
	.byte	0x1
	.value	0x111
	.long	0xea
	.uleb128 0x25
	.long	.LASF53
	.byte	0x1
	.value	0x113
	.long	0xea
	.uleb128 0x25
	.long	.LASF59
	.byte	0x1
	.value	0x115
	.long	0xea
	.uleb128 0x25
	.long	.LASF60
	.byte	0x1
	.value	0x117
	.long	0xea
	.uleb128 0x25
	.long	.LASF61
	.byte	0x1
	.value	0x119
	.long	0xea
	.byte	0x0
	.byte	0x0
	.uleb128 0x26
	.byte	0x1
	.long	.LASF62
	.byte	0x1
	.value	0x12a
	.byte	0x1
	.long	0xd8
	.quad	.LFB35
	.quad	.LFE35
	.long	.LLST58
	.long	0x829
	.uleb128 0x27
	.string	"t"
	.byte	0x1
	.value	0x129
	.long	0x57
	.long	.LLST59
	.byte	0x0
	.uleb128 0x28
	.byte	0x1
	.long	.LASF64
	.byte	0x1
	.value	0x130
	.byte	0x1
	.quad	.LFB36
	.quad	.LFE36
	.long	.LLST60
	.long	0x934
	.uleb128 0x27
	.string	"fun"
	.byte	0x1
	.value	0x130
	.long	0x13c
	.long	.LLST61
	.uleb128 0x29
	.long	.LASF17
	.byte	0x1
	.value	0x130
	.long	0x68
	.long	.LLST62
	.uleb128 0x29
	.long	.LASF18
	.byte	0x1
	.value	0x130
	.long	0x68
	.long	.LLST63
	.uleb128 0x2a
	.string	"t"
	.byte	0x1
	.value	0x131
	.long	0x57
	.long	.LLST64
	.uleb128 0x2b
	.string	"cyc"
	.byte	0x1
	.value	0x132
	.long	0xd8
	.byte	0x2
	.byte	0x77
	.sleb128 0
	.uleb128 0x25
	.long	.LASF63
	.byte	0x1
	.value	0x132
	.long	0xd8
	.uleb128 0x2c
	.string	"ghz"
	.byte	0x1
	.value	0x132
	.long	0xd8
	.uleb128 0x2d
	.quad	.LBB17
	.quad	.LBE17
	.long	0x915
	.uleb128 0x2b
	.string	"val"
	.byte	0x1
	.value	0x134
	.long	0xd8
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.uleb128 0x2e
	.long	0x1a1
	.quad	.LBB18
	.quad	.LBE18
	.byte	0x1
	.value	0x136
	.long	0x8f6
	.uleb128 0x2f
	.long	0x1b3
	.byte	0x0
	.uleb128 0x30
	.long	0x1a1
	.quad	.LBB20
	.quad	.LBE20
	.byte	0x1
	.value	0x137
	.uleb128 0x2f
	.long	0x1b3
	.byte	0x0
	.byte	0x0
	.uleb128 0x30
	.long	0x1a1
	.quad	.LBB22
	.quad	.LBE22
	.byte	0x1
	.value	0x140
	.uleb128 0x2f
	.long	0x1b3
	.byte	0x0
	.byte	0x0
	.uleb128 0x31
	.byte	0x1
	.long	.LASF65
	.byte	0x1
	.byte	0x1c
	.byte	0x1
	.quad	.LFB19
	.quad	.LFE19
	.long	.LLST65
	.long	0x9b8
	.uleb128 0x1c
	.string	"cnt"
	.byte	0x1
	.byte	0x1c
	.long	0x57
	.long	.LLST66
	.uleb128 0x1e
	.string	"t"
	.byte	0x1
	.byte	0x1f
	.long	0x57
	.uleb128 0x32
	.quad	.LBB24
	.quad	.LBE24
	.uleb128 0x1b
	.string	"i"
	.byte	0x1
	.byte	0x22
	.long	0x57
	.long	.LLST67
	.uleb128 0x23
	.long	.LASF66
	.byte	0x1
	.byte	0x24
	.long	0x9b8
	.uleb128 0x22
	.long	.Ldebug_ranges0+0x90
	.uleb128 0x1b
	.string	"val"
	.byte	0x1
	.byte	0x27
	.long	0xd8
	.long	.LLST68
	.uleb128 0x23
	.long	.LASF67
	.byte	0x1
	.byte	0x29
	.long	0x9e
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0x5
	.byte	0x8
	.long	0xdf
	.uleb128 0x33
	.byte	0x1
	.string	"run"
	.byte	0x1
	.value	0x157
	.byte	0x1
	.quad	.LFB37
	.quad	.LFE37
	.long	.LLST69
	.long	0xa19
	.uleb128 0x27
	.string	"cnt"
	.byte	0x1
	.value	0x157
	.long	0x57
	.long	.LLST70
	.uleb128 0x2c
	.string	"i"
	.byte	0x1
	.value	0x15a
	.long	0x57
	.uleb128 0x30
	.long	0x1a1
	.quad	.LBB27
	.quad	.LBE27
	.byte	0x1
	.value	0x159
	.uleb128 0x2f
	.long	0x1b3
	.byte	0x0
	.byte	0x0
	.uleb128 0x26
	.byte	0x1
	.long	.LASF68
	.byte	0x1
	.value	0x160
	.byte	0x1
	.long	0x57
	.quad	.LFB38
	.quad	.LFE38
	.long	.LLST71
	.long	0xa70
	.uleb128 0x29
	.long	.LASF69
	.byte	0x1
	.value	0x160
	.long	0x57
	.long	.LLST72
	.uleb128 0x29
	.long	.LASF70
	.byte	0x1
	.value	0x160
	.long	0xa70
	.long	.LLST73
	.uleb128 0x2a
	.string	"cnt"
	.byte	0x1
	.value	0x161
	.long	0x57
	.long	.LLST74
	.byte	0x0
	.uleb128 0x5
	.byte	0x8
	.long	0x68
	.uleb128 0x7
	.long	0x289
	.long	0xa86
	.uleb128 0x8
	.long	0x65
	.byte	0x4
	.byte	0x0
	.uleb128 0x34
	.long	.LASF71
	.byte	0x1
	.byte	0x14
	.long	0xa76
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	array_data
	.uleb128 0x7
	.long	0x9e
	.long	0xaac
	.uleb128 0x8
	.long	0x65
	.byte	0x4
	.byte	0x0
	.uleb128 0x34
	.long	.LASF72
	.byte	0x1
	.byte	0x15
	.long	0xa9c
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	list_data
	.uleb128 0x34
	.long	.LASF73
	.byte	0x1
	.byte	0x18
	.long	0x57
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	global_cnt
	.uleb128 0x7
	.long	0xd8
	.long	0xae8
	.uleb128 0x8
	.long	0x65
	.byte	0x4
	.byte	0x0
	.uleb128 0x34
	.long	.LASF74
	.byte	0x1
	.byte	0x1a
	.long	0xad8
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	answer
	.uleb128 0x7
	.long	0x6e
	.long	0xb0e
	.uleb128 0x8
	.long	0x65
	.byte	0x1d
	.byte	0x0
	.uleb128 0x34
	.long	.LASF75
	.byte	0x1
	.byte	0x32
	.long	0xafe
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	list_prod_descr
	.uleb128 0x34
	.long	.LASF76
	.byte	0x1
	.byte	0x42
	.long	0x80
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	array_u1p1S_prod_descr
	.uleb128 0x34
	.long	.LASF77
	.byte	0x1
	.byte	0x52
	.long	0x80
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	array_u5p5S_prod_descr
	.uleb128 0x34
	.long	.LASF78
	.byte	0x1
	.byte	0x6b
	.long	0x80
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	array_u5p1S_prod_descr
	.uleb128 0x34
	.long	.LASF79
	.byte	0x1
	.byte	0x7d
	.long	0x80
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	array_u5p1A_prod_descr
	.uleb128 0x7
	.long	0x6e
	.long	0xb8c
	.uleb128 0x8
	.long	0x65
	.byte	0x21
	.byte	0x0
	.uleb128 0x34
	.long	.LASF80
	.byte	0x1
	.byte	0x99
	.long	0xb7c
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	sse_u1p1_prod_descr
	.uleb128 0x34
	.long	.LASF81
	.byte	0x1
	.byte	0xc4
	.long	0xb7c
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	sse_u5p5_prod_descr
	.uleb128 0x34
	.long	.LASF82
	.byte	0x1
	.byte	0xf6
	.long	0xb7c
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	sse_u8p8_prod_descr
	.uleb128 0x7
	.long	0x195
	.long	0xbde
	.uleb128 0x8
	.long	0x65
	.byte	0x8
	.byte	0x0
	.uleb128 0x35
	.long	.LASF83
	.byte	0x1
	.value	0x14a
	.long	0xbce
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	tests
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
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x9
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
	.uleb128 0xa
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
	.uleb128 0xb
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
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
	.uleb128 0xd
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x2107
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xe
	.uleb128 0x17
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
	.uleb128 0xf
	.uleb128 0xd
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
	.uleb128 0x10
	.uleb128 0x16
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x11
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
	.uleb128 0x12
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x13
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x14
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
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
	.uleb128 0x15
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
	.uleb128 0x16
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
	.uleb128 0x17
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
	.uleb128 0x18
	.uleb128 0x18
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0x19
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
	.uleb128 0x1a
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
	.uleb128 0x1d
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
	.uleb128 0x1e
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
	.uleb128 0x1f
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
	.uleb128 0x20
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
	.uleb128 0x21
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
	.uleb128 0x22
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x23
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
	.uleb128 0x25
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x26
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
	.uleb128 0x27
	.uleb128 0x5
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
	.uleb128 0x28
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
	.uleb128 0x29
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
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
	.uleb128 0x2a
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
	.uleb128 0x2b
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
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x2c
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
	.uleb128 0x2d
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x2e
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
	.uleb128 0x2f
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x30
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
	.uleb128 0x31
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
	.uleb128 0x32
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x33
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
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
	.uleb128 0x34
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
	.uleb128 0x35
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
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
	.long	0x294
	.value	0x2
	.long	.Ldebug_info0
	.long	0xbf6
	.long	0x1c0
	.string	"list_prod"
	.long	0x200
	.string	"wrap_list_prod"
	.long	0x232
	.string	"array_u1p1S_prod"
	.long	0x28f
	.string	"wrap_array_u1p1S_prod"
	.long	0x2c1
	.string	"array_u5p5S_prod"
	.long	0x358
	.string	"wrap_array_u5p5S_prod"
	.long	0x38a
	.string	"array_u5p1S_prod"
	.long	0x3e5
	.string	"wrap_array_u5p1S_prod"
	.long	0x417
	.string	"array_u5p1A_prod"
	.long	0x472
	.string	"wrap_array_u5p1A_prod"
	.long	0x4a4
	.string	"sse_u1p1_prod"
	.long	0x53a
	.string	"wrap_sse_u1p1_prod"
	.long	0x56d
	.string	"sse_u5p5_prod"
	.long	0x66b
	.string	"wrap_sse_u5p5_prod"
	.long	0x69e
	.string	"sse_u8p8_prod"
	.long	0x7f4
	.string	"wrap_sse_u8p8_prod"
	.long	0x829
	.string	"tester"
	.long	0x934
	.string	"init"
	.long	0x9be
	.string	"run"
	.long	0xa19
	.string	"main"
	.long	0xa86
	.string	"array_data"
	.long	0xaac
	.string	"list_data"
	.long	0xac2
	.string	"global_cnt"
	.long	0xae8
	.string	"answer"
	.long	0xb0e
	.string	"list_prod_descr"
	.long	0xb24
	.string	"array_u1p1S_prod_descr"
	.long	0xb3a
	.string	"array_u5p5S_prod_descr"
	.long	0xb50
	.string	"array_u5p1S_prod_descr"
	.long	0xb66
	.string	"array_u5p1A_prod_descr"
	.long	0xb8c
	.string	"sse_u1p1_prod_descr"
	.long	0xba2
	.string	"sse_u5p5_prod_descr"
	.long	0xbb8
	.string	"sse_u8p8_prod_descr"
	.long	0xbde
	.string	"tests"
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
	.quad	.LBB11-.Ltext0
	.quad	.LBE11-.Ltext0
	.quad	.LBB12-.Ltext0
	.quad	.LBE12-.Ltext0
	.quad	0x0
	.quad	0x0
	.quad	.LBB13-.Ltext0
	.quad	.LBE13-.Ltext0
	.quad	.LBB14-.Ltext0
	.quad	.LBE14-.Ltext0
	.quad	0x0
	.quad	0x0
	.quad	.LBB15-.Ltext0
	.quad	.LBE15-.Ltext0
	.quad	.LBB16-.Ltext0
	.quad	.LBE16-.Ltext0
	.quad	0x0
	.quad	0x0
	.quad	.LBB25-.Ltext0
	.quad	.LBE25-.Ltext0
	.quad	.LBB26-.Ltext0
	.quad	.LBE26-.Ltext0
	.quad	0x0
	.quad	0x0
	.section	.debug_str,"MS",@progbits,1
.LASF87:
	.string	"printf"
.LASF65:
	.string	"init"
.LASF8:
	.string	"long long unsigned int"
.LASF10:
	.string	"next"
.LASF66:
	.string	"list_nodes"
.LASF9:
	.string	"long long int"
.LASF4:
	.string	"signed char"
.LASF75:
	.string	"list_prod_descr"
.LASF71:
	.string	"array_data"
.LASF6:
	.string	"long int"
.LASF27:
	.string	"val1"
.LASF38:
	.string	"accum"
.LASF11:
	.string	"double"
.LASF83:
	.string	"tests"
.LASF23:
	.string	"array"
.LASF21:
	.string	"wrap_list_prod"
.LASF84:
	.string	"GNU C 4.3.2"
.LASF49:
	.string	"chunk0"
.LASF50:
	.string	"chunk1"
.LASF51:
	.string	"chunk2"
.LASF52:
	.string	"chunk3"
.LASF53:
	.string	"chunk4"
.LASF59:
	.string	"chunk5"
.LASF55:
	.string	"sse_u8p8_prod"
.LASF61:
	.string	"chunk7"
.LASF3:
	.string	"unsigned int"
.LASF76:
	.string	"array_u1p1S_prod_descr"
.LASF0:
	.string	"long unsigned int"
.LASF62:
	.string	"wrap_sse_u8p8_prod"
.LASF17:
	.string	"name"
.LASF40:
	.string	"data"
.LASF2:
	.string	"short unsigned int"
.LASF20:
	.string	"list_prod"
.LASF72:
	.string	"list_data"
.LASF74:
	.string	"answer"
.LASF81:
	.string	"sse_u5p5_prod_descr"
.LASF64:
	.string	"tester"
.LASF34:
	.string	"array_u5p1A_prod"
.LASF33:
	.string	"wrap_array_u5p1S_prod"
.LASF32:
	.string	"array_u5p1S_prod"
.LASF73:
	.string	"global_cnt"
.LASF54:
	.string	"wrap_sse_u5p5_prod"
.LASF26:
	.string	"val0"
.LASF25:
	.string	"array_u5p5S_prod"
.LASF28:
	.string	"val2"
.LASF29:
	.string	"val3"
.LASF30:
	.string	"val4"
.LASF78:
	.string	"array_u5p1S_prod_descr"
.LASF85:
	.string	"dmult.c"
.LASF44:
	.string	"accum0"
.LASF45:
	.string	"accum1"
.LASF46:
	.string	"accum2"
.LASF47:
	.string	"accum3"
.LASF48:
	.string	"accum4"
.LASF56:
	.string	"accum5"
.LASF57:
	.string	"accum6"
.LASF58:
	.string	"accum7"
.LASF77:
	.string	"array_u5p5S_prod_descr"
.LASF14:
	.string	"vec_t"
.LASF16:
	.string	"dfun"
.LASF37:
	.string	"xfer"
.LASF18:
	.string	"descr"
.LASF1:
	.string	"unsigned char"
.LASF5:
	.string	"short int"
.LASF36:
	.string	"sse_u1p1_prod"
.LASF13:
	.string	"list_ele"
.LASF80:
	.string	"sse_u1p1_prod_descr"
.LASF42:
	.string	"wrap_sse_u1p1_prod"
.LASF67:
	.string	"head"
.LASF19:
	.string	"b_ele"
.LASF41:
	.string	"chunk"
.LASF7:
	.string	"char"
.LASF15:
	.string	"pack_t"
.LASF12:
	.string	"list_ptr"
.LASF35:
	.string	"wrap_array_u5p1A_prod"
.LASF82:
	.string	"sse_u8p8_prod_descr"
.LASF88:
	.string	"__fmt"
.LASF60:
	.string	"chunk6"
.LASF43:
	.string	"sse_u5p5_prod"
.LASF31:
	.string	"wrap_array_u5p5S_prod"
.LASF70:
	.string	"argv"
.LASF24:
	.string	"wrap_array_u1p1S_prod"
.LASF22:
	.string	"array_u1p1S_prod"
.LASF79:
	.string	"array_u5p1A_prod_descr"
.LASF63:
	.string	"nsecs"
.LASF69:
	.string	"argc"
.LASF68:
	.string	"main"
.LASF39:
	.string	"result"
.LASF86:
	.string	"/home/rbryant/ics2/code/opt"
	.ident	"GCC: (Ubuntu 4.3.2-1ubuntu11) 4.3.2"
	.section	.note.GNU-stack,"",@progbits
