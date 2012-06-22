	.file	"combine.c"
	.text
	.p2align 4,,15
.globl register_combiners
	.type	register_combiners, @function
register_combiners:
.LFB55:
	movl	$combine1, %esi
	subq	$8, %rsp
.LCFI0:
	movl	$combine1_descr, %edx
	movq	%rsi, %rdi
	call	add_combiner
	movl	$combine2_descr, %edx
	movl	$combine1, %esi
	movl	$combine2, %edi
	call	add_combiner
	movl	$combine3_descr, %edx
	movl	$combine1, %esi
	movl	$combine3, %edi
	call	add_combiner
	movl	$combine3w_descr, %edx
	movl	$combine1, %esi
	movl	$combine3w, %edi
	call	add_combiner
	movl	$combine4_descr, %edx
	movl	$combine1, %esi
	movl	$combine4, %edi
	call	add_combiner
	movl	$combine4b_descr, %edx
	movl	$combine1, %esi
	movl	$combine4b, %edi
	call	add_combiner
	movl	$combine4p_descr, %edx
	movl	$combine1, %esi
	movl	$combine4p, %edi
	call	add_combiner
	movl	$combine5_descr, %edx
	movl	$combine1, %esi
	movl	$combine5, %edi
	call	add_combiner
	movl	$combine5p_descr, %edx
	movl	$combine1, %esi
	movl	$combine5p, %edi
	call	add_combiner
	movl	$unroll2aw_descr, %edx
	movl	$combine1, %esi
	movl	$unroll2aw_combine, %edi
	call	add_combiner
	movl	$unroll3a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll3a_combine, %edi
	call	add_combiner
	movl	$unroll4a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll4a_combine, %edi
	call	add_combiner
	movl	$unroll5a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll5a_combine, %edi
	call	add_combiner
	movl	$unroll6a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll6a_combine, %edi
	call	add_combiner
	movl	$unroll8a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll8a_combine, %edi
	call	add_combiner
	movl	$unroll16a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll16a_combine, %edi
	call	add_combiner
	movl	$unroll2_descr, %edx
	movl	$combine1, %esi
	movl	$unroll2_combine, %edi
	call	add_combiner
	movl	$unroll3_descr, %edx
	movl	$combine1, %esi
	movl	$unroll3_combine, %edi
	call	add_combiner
	movl	$unroll4_descr, %edx
	movl	$combine1, %esi
	movl	$unroll4_combine, %edi
	call	add_combiner
	movl	$unroll8_descr, %edx
	movl	$combine1, %esi
	movl	$unroll8_combine, %edi
	call	add_combiner
	movl	$unroll16_descr, %edx
	movl	$combine1, %esi
	movl	$unroll16_combine, %edi
	call	add_combiner
	movl	$combine6_descr, %edx
	movl	$combine1, %esi
	movl	$combine6, %edi
	call	add_combiner
	movl	$unroll4x2a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll4x2a_combine, %edi
	call	add_combiner
	movl	$unroll8x2a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll8x2a_combine, %edi
	call	add_combiner
	movl	$unroll3x3a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll3x3a_combine, %edi
	call	add_combiner
	movl	$unroll4x4a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll4x4a_combine, %edi
	call	add_combiner
	movl	$unroll5x5a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll5x5a_combine, %edi
	call	add_combiner
	movl	$unroll6x6a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll6x6a_combine, %edi
	call	add_combiner
	movl	$unroll8x4a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll8x4a_combine, %edi
	call	add_combiner
	movl	$unroll8x8a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll8x8a_combine, %edi
	call	add_combiner
	movl	$unroll10x10a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll10x10a_combine, %edi
	call	add_combiner
	movl	$unroll12x6a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll12x6a_combine, %edi
	call	add_combiner
	movl	$unroll12x12a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll12x12a_combine, %edi
	call	add_combiner
	movl	$unroll8x2_descr, %edx
	movl	$combine1, %esi
	movl	$unroll8x2_combine, %edi
	call	add_combiner
	movl	$unroll8x4_descr, %edx
	movl	$combine1, %esi
	movl	$unroll8x4_combine, %edi
	call	add_combiner
	movl	$unroll8x8_descr, %edx
	movl	$combine1, %esi
	movl	$unroll8x8_combine, %edi
	call	add_combiner
	movl	$unroll9x3_descr, %edx
	movl	$combine1, %esi
	movl	$unroll9x3_combine, %edi
	call	add_combiner
	movl	$unrollx2as_descr, %edx
	movl	$combine1, %esi
	movl	$unrollx2as_combine, %edi
	call	add_combiner
	movl	$combine7_descr, %edx
	movl	$combine1, %esi
	movl	$combine7, %edi
	call	add_combiner
	movl	$unroll3aa_descr, %edx
	movl	$combine1, %esi
	movl	$unroll3aa_combine, %edi
	call	add_combiner
	movl	$unroll4aa_descr, %edx
	movl	$combine1, %esi
	movl	$unroll4aa_combine, %edi
	call	add_combiner
	movl	$unroll5aa_descr, %edx
	movl	$combine1, %esi
	movl	$unroll5aa_combine, %edi
	call	add_combiner
	movl	$unroll6aa_descr, %edx
	movl	$combine1, %esi
	movl	$unroll6aa_combine, %edi
	call	add_combiner
	movl	$unroll8aa_descr, %edx
	movl	$combine1, %esi
	movl	$unroll8aa_combine, %edi
	call	add_combiner
	movl	$simd_v1_descr, %edx
	movl	$combine1, %esi
	movl	$simd_v1_combine, %edi
	call	add_combiner
	movl	$simd_v2_descr, %edx
	movl	$combine1, %esi
	movl	$simd_v2_combine, %edi
	call	add_combiner
	movl	$simd_v4_descr, %edx
	movl	$combine1, %esi
	movl	$simd_v4_combine, %edi
	call	add_combiner
	movl	$simd_v8_descr, %edx
	movl	$combine1, %esi
	movl	$simd_v8_combine, %edi
	call	add_combiner
	movl	$simd_v12_descr, %edx
	movl	$combine1, %esi
	movl	$simd_v12_combine, %edi
	call	add_combiner
	movl	$simd_v2a_descr, %edx
	movl	$combine1, %esi
	movl	$simd_v2a_combine, %edi
	call	add_combiner
	movl	$simd_v4a_descr, %edx
	movl	$combine1, %esi
	movl	$simd_v4a_combine, %edi
	call	add_combiner
	movl	$simd_v8a_combine, %edi
	movl	$simd_v8a_descr, %edx
	movl	$combine1, %esi
	call	add_combiner
	movsd	.LC0(%rip), %xmm1
	movl	$simd_v8a_combine, %edi
	movsd	.LC1(%rip), %xmm0
	addq	$8, %rsp
	jmp	log_combiner
.LFE55:
	.size	register_combiners, .-register_combiners
	.p2align 4,,15
.globl simd_v8a_combine
	.type	simd_v8a_combine, @function
simd_v8a_combine:
.LFB54:
	pushq	%r12
.LCFI1:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI2:
	pushq	%rbx
.LCFI3:
	movq	%rdi, %rbx
	subq	$16, %rsp
.LCFI4:
	call	get_vec_start
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	vec_length
	testb	$15, %bpl
	movl	%eax, %ecx
	jne	.L20
.L15:
	movss	.LC2(%rip), %xmm3
	.p2align 4,,10
	.p2align 3
.L6:
	cmpl	$31, %ecx
	jle	.L21
	movl	%ecx, %edi
	movaps	.LC3(%rip), %xmm4
	leal	-32(%rdi), %esi
	movq	%rbp, %rdx
	movl	%esi, %ecx
	shrl	$5, %ecx
	mov	%ecx, %eax
	salq	$7, %rax
	leaq	128(%rbp,%rax), %rax
	.p2align 4,,10
	.p2align 3
.L11:
	movaps	(%rdx), %xmm2
	movaps	32(%rdx), %xmm0
	mulps	16(%rdx), %xmm2
	movaps	64(%rdx), %xmm1
	mulps	48(%rdx), %xmm0
	mulps	80(%rdx), %xmm1
	mulps	%xmm0, %xmm2
	movaps	96(%rdx), %xmm0
	mulps	112(%rdx), %xmm0
	subq	$-128, %rdx
	cmpq	%rax, %rdx
	mulps	%xmm0, %xmm1
	mulps	%xmm1, %xmm2
	mulps	%xmm2, %xmm4
	jne	.L11
	leal	-32(%rdi), %eax
	sall	$5, %ecx
	subl	%ecx, %esi
	shrl	$5, %eax
	movl	%esi, %ecx
	mov	%eax, %eax
	salq	$7, %rax
	leaq	128(%rbp,%rax), %rbp
.L10:
	testl	%ecx, %ecx
	je	.L12
	leal	-1(%rcx), %eax
	movq	%rbp, %rdx
	leaq	4(%rbp,%rax,4), %rax
	.p2align 4,,10
	.p2align 3
.L13:
	mulss	(%rdx), %xmm3
	addq	$4, %rdx
	cmpq	%rax, %rdx
	jne	.L13
.L12:
	movaps	%xmm4, (%rsp)
	mulss	(%rsp), %xmm3
	mulss	4(%rsp), %xmm3
	mulss	8(%rsp), %xmm3
	mulss	12(%rsp), %xmm3
	movss	%xmm3, (%r12)
	addq	$16, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L20:
	testl	%eax, %eax
	je	.L15
	movss	.LC2(%rip), %xmm3
.L7:
	mulss	(%rbp), %xmm3
	addq	$4, %rbp
	subl	$1, %ecx
	testb	$15, %bpl
	je	.L6
	testl	%ecx, %ecx
	jne	.L7
	jmp	.L6
.L21:
	movaps	.LC3(%rip), %xmm4
	.p2align 4,,4
	.p2align 3
	jmp	.L10
.LFE54:
	.size	simd_v8a_combine, .-simd_v8a_combine
	.p2align 4,,15
.globl simd_v4a_combine
	.type	simd_v4a_combine, @function
simd_v4a_combine:
.LFB53:
	pushq	%r12
.LCFI5:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI6:
	pushq	%rbx
.LCFI7:
	movq	%rdi, %rbx
	subq	$16, %rsp
.LCFI8:
	call	get_vec_start
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	vec_length
	testb	$15, %bpl
	movl	%eax, %ecx
	jne	.L38
.L34:
	movss	.LC2(%rip), %xmm2
	.p2align 4,,10
	.p2align 3
.L25:
	cmpl	$15, %ecx
	jle	.L39
	movl	%ecx, %edi
	movaps	.LC3(%rip), %xmm3
	leal	-16(%rdi), %esi
	movq	%rbp, %rdx
	movl	%esi, %ecx
	shrl	$4, %ecx
	mov	%ecx, %eax
	salq	$6, %rax
	leaq	64(%rbp,%rax), %rax
	.p2align 4,,10
	.p2align 3
.L30:
	movaps	(%rdx), %xmm1
	movaps	32(%rdx), %xmm0
	mulps	16(%rdx), %xmm1
	mulps	48(%rdx), %xmm0
	addq	$64, %rdx
	cmpq	%rax, %rdx
	mulps	%xmm0, %xmm1
	mulps	%xmm1, %xmm3
	jne	.L30
	leal	-16(%rdi), %eax
	sall	$4, %ecx
	subl	%ecx, %esi
	shrl	$4, %eax
	movl	%esi, %ecx
	mov	%eax, %eax
	salq	$6, %rax
	leaq	64(%rbp,%rax), %rbp
.L29:
	testl	%ecx, %ecx
	je	.L31
	leal	-1(%rcx), %eax
	movq	%rbp, %rdx
	leaq	4(%rbp,%rax,4), %rax
	.p2align 4,,10
	.p2align 3
.L32:
	mulss	(%rdx), %xmm2
	addq	$4, %rdx
	cmpq	%rax, %rdx
	jne	.L32
.L31:
	movaps	%xmm3, (%rsp)
	mulss	(%rsp), %xmm2
	mulss	4(%rsp), %xmm2
	mulss	8(%rsp), %xmm2
	mulss	12(%rsp), %xmm2
	movss	%xmm2, (%r12)
	addq	$16, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L38:
	testl	%eax, %eax
	je	.L34
	movss	.LC2(%rip), %xmm2
.L26:
	mulss	(%rbp), %xmm2
	addq	$4, %rbp
	subl	$1, %ecx
	testb	$15, %bpl
	je	.L25
	testl	%ecx, %ecx
	jne	.L26
	jmp	.L25
.L39:
	movaps	.LC3(%rip), %xmm3
	.p2align 4,,4
	.p2align 3
	jmp	.L29
.LFE53:
	.size	simd_v4a_combine, .-simd_v4a_combine
	.p2align 4,,15
.globl simd_v2a_combine
	.type	simd_v2a_combine, @function
simd_v2a_combine:
.LFB52:
	pushq	%r12
.LCFI9:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI10:
	pushq	%rbx
.LCFI11:
	movq	%rdi, %rbx
	subq	$16, %rsp
.LCFI12:
	call	get_vec_start
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	vec_length
	testb	$15, %bpl
	movl	%eax, %ecx
	jne	.L56
.L52:
	movss	.LC2(%rip), %xmm1
	.p2align 4,,10
	.p2align 3
.L43:
	cmpl	$7, %ecx
	jle	.L57
	movl	%ecx, %edi
	movaps	.LC3(%rip), %xmm2
	leal	-8(%rdi), %esi
	movq	%rbp, %rdx
	movl	%esi, %ecx
	shrl	$3, %ecx
	mov	%ecx, %eax
	salq	$5, %rax
	leaq	32(%rbp,%rax), %rax
	.p2align 4,,10
	.p2align 3
.L48:
	movaps	(%rdx), %xmm0
	mulps	16(%rdx), %xmm0
	addq	$32, %rdx
	cmpq	%rax, %rdx
	mulps	%xmm0, %xmm2
	jne	.L48
	leal	0(,%rcx,8), %eax
	movl	%esi, %ecx
	subl	%eax, %ecx
	leal	-8(%rdi), %eax
	shrl	$3, %eax
	mov	%eax, %eax
	salq	$5, %rax
	leaq	32(%rbp,%rax), %rbp
.L47:
	testl	%ecx, %ecx
	je	.L49
	leal	-1(%rcx), %eax
	movq	%rbp, %rdx
	leaq	4(%rbp,%rax,4), %rax
	.p2align 4,,10
	.p2align 3
.L50:
	mulss	(%rdx), %xmm1
	addq	$4, %rdx
	cmpq	%rax, %rdx
	jne	.L50
.L49:
	movaps	%xmm2, (%rsp)
	mulss	(%rsp), %xmm1
	mulss	4(%rsp), %xmm1
	mulss	8(%rsp), %xmm1
	mulss	12(%rsp), %xmm1
	movss	%xmm1, (%r12)
	addq	$16, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L56:
	testl	%eax, %eax
	je	.L52
	movss	.LC2(%rip), %xmm1
.L44:
	mulss	(%rbp), %xmm1
	addq	$4, %rbp
	subl	$1, %ecx
	testb	$15, %bpl
	je	.L43
	testl	%ecx, %ecx
	jne	.L44
	jmp	.L43
.L57:
	movaps	.LC3(%rip), %xmm2
	.p2align 4,,4
	.p2align 3
	jmp	.L47
.LFE52:
	.size	simd_v2a_combine, .-simd_v2a_combine
	.p2align 4,,15
.globl simd_v12_combine
	.type	simd_v12_combine, @function
simd_v12_combine:
.LFB51:
	pushq	%r12
.LCFI13:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI14:
	pushq	%rbx
.LCFI15:
	movq	%rdi, %rbx
	subq	$16, %rsp
.LCFI16:
	call	get_vec_start
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	vec_length
	testb	$15, %bpl
	movl	%eax, %ecx
	jne	.L74
.L70:
	movss	.LC2(%rip), %xmm6
	.p2align 4,,10
	.p2align 3
.L61:
	cmpl	$47, %ecx
	jle	.L75
	movl	%ecx, %edi
	movl	$-1431655765, %eax
	movq	%rbp, %r8
	leal	-48(%rdi), %esi
	movaps	.LC3(%rip), %xmm0
	mull	%esi
	movaps	%xmm0, %xmm12
	movaps	%xmm0, %xmm10
	movl	%edx, %ecx
	movaps	%xmm0, %xmm11
	shrl	$5, %ecx
	movaps	%xmm0, %xmm8
	mov	%ecx, %eax
	movaps	%xmm0, %xmm9
	leaq	3(%rax,%rax,2), %rax
	movaps	%xmm0, %xmm5
	movaps	%xmm0, %xmm7
	salq	$6, %rax
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm4
	leaq	(%rbp,%rax), %rax
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm2
	.p2align 4,,10
	.p2align 3
.L66:
	mulps	(%r8), %xmm0
	mulps	16(%r8), %xmm12
	mulps	32(%r8), %xmm10
	mulps	48(%r8), %xmm11
	mulps	64(%r8), %xmm8
	mulps	80(%r8), %xmm9
	mulps	96(%r8), %xmm5
	mulps	112(%r8), %xmm7
	mulps	128(%r8), %xmm3
	mulps	144(%r8), %xmm4
	mulps	160(%r8), %xmm1
	mulps	176(%r8), %xmm2
	addq	$192, %r8
	cmpq	%rax, %r8
	jne	.L66
	leal	(%rcx,%rcx,2), %eax
	leal	-48(%rdi), %edx
	movl	%esi, %ecx
	sall	$4, %eax
	subl	%eax, %ecx
	movl	$-1431655765, %eax
	mull	%edx
	shrl	$5, %edx
	mov	%edx, %edx
	leaq	3(%rdx,%rdx,2), %rdx
	salq	$6, %rdx
	addq	%rdx, %rbp
.L65:
	testl	%ecx, %ecx
	je	.L67
	leal	-1(%rcx), %eax
	movq	%rbp, %rdx
	leaq	4(%rbp,%rax,4), %rax
	.p2align 4,,10
	.p2align 3
.L68:
	mulss	(%rdx), %xmm6
	addq	$4, %rdx
	cmpq	%rax, %rdx
	jne	.L68
.L67:
	mulps	%xmm12, %xmm0
	mulps	%xmm11, %xmm10
	mulps	%xmm9, %xmm8
	mulps	%xmm7, %xmm5
	mulps	%xmm10, %xmm0
	mulps	%xmm4, %xmm3
	mulps	%xmm2, %xmm1
	mulps	%xmm8, %xmm0
	mulps	%xmm5, %xmm0
	mulps	%xmm3, %xmm0
	mulps	%xmm1, %xmm0
	movaps	%xmm0, (%rsp)
	mulss	(%rsp), %xmm6
	mulss	4(%rsp), %xmm6
	mulss	8(%rsp), %xmm6
	mulss	12(%rsp), %xmm6
	movss	%xmm6, (%r12)
	addq	$16, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L74:
	testl	%eax, %eax
	je	.L70
	movss	.LC2(%rip), %xmm6
.L62:
	mulss	(%rbp), %xmm6
	addq	$4, %rbp
	subl	$1, %ecx
	testb	$15, %bpl
	je	.L61
	testl	%ecx, %ecx
	jne	.L62
	jmp	.L61
.L75:
	movaps	.LC3(%rip), %xmm0
	movaps	%xmm0, %xmm12
	movaps	%xmm0, %xmm10
	movaps	%xmm0, %xmm11
	movaps	%xmm0, %xmm8
	movaps	%xmm0, %xmm9
	movaps	%xmm0, %xmm5
	movaps	%xmm0, %xmm7
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm4
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm2
	jmp	.L65
.LFE51:
	.size	simd_v12_combine, .-simd_v12_combine
	.p2align 4,,15
.globl simd_v8_combine
	.type	simd_v8_combine, @function
simd_v8_combine:
.LFB50:
	pushq	%r12
.LCFI17:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI18:
	pushq	%rbx
.LCFI19:
	movq	%rdi, %rbx
	subq	$16, %rsp
.LCFI20:
	call	get_vec_start
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	vec_length
	testb	$15, %bpl
	movl	%eax, %ecx
	jne	.L92
.L88:
	movss	.LC2(%rip), %xmm7
	.p2align 4,,10
	.p2align 3
.L79:
	cmpl	$31, %ecx
	jle	.L93
	movl	%ecx, %edi
	movaps	.LC3(%rip), %xmm0
	leal	-32(%rdi), %esi
	movq	%rbp, %rdx
	movaps	%xmm0, %xmm8
	movaps	%xmm0, %xmm5
	movl	%esi, %ecx
	movaps	%xmm0, %xmm6
	shrl	$5, %ecx
	movaps	%xmm0, %xmm3
	mov	%ecx, %eax
	movaps	%xmm0, %xmm4
	salq	$7, %rax
	movaps	%xmm0, %xmm2
	movaps	%xmm0, %xmm1
	leaq	128(%rbp,%rax), %rax
	.p2align 4,,10
	.p2align 3
.L84:
	mulps	(%rdx), %xmm0
	mulps	16(%rdx), %xmm8
	mulps	32(%rdx), %xmm5
	mulps	48(%rdx), %xmm6
	mulps	64(%rdx), %xmm3
	mulps	80(%rdx), %xmm4
	mulps	96(%rdx), %xmm2
	mulps	112(%rdx), %xmm1
	subq	$-128, %rdx
	cmpq	%rax, %rdx
	jne	.L84
	leal	-32(%rdi), %eax
	sall	$5, %ecx
	subl	%ecx, %esi
	shrl	$5, %eax
	movl	%esi, %ecx
	mov	%eax, %eax
	salq	$7, %rax
	leaq	128(%rbp,%rax), %rbp
.L83:
	testl	%ecx, %ecx
	je	.L85
	leal	-1(%rcx), %eax
	movq	%rbp, %rdx
	leaq	4(%rbp,%rax,4), %rax
	.p2align 4,,10
	.p2align 3
.L86:
	mulss	(%rdx), %xmm7
	addq	$4, %rdx
	cmpq	%rax, %rdx
	jne	.L86
.L85:
	mulps	%xmm8, %xmm0
	mulps	%xmm6, %xmm5
	mulps	%xmm4, %xmm3
	mulps	%xmm1, %xmm2
	mulps	%xmm5, %xmm0
	mulps	%xmm3, %xmm0
	mulps	%xmm2, %xmm0
	movaps	%xmm0, (%rsp)
	mulss	(%rsp), %xmm7
	mulss	4(%rsp), %xmm7
	mulss	8(%rsp), %xmm7
	mulss	12(%rsp), %xmm7
	movss	%xmm7, (%r12)
	addq	$16, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L92:
	testl	%eax, %eax
	je	.L88
	movss	.LC2(%rip), %xmm7
.L80:
	mulss	(%rbp), %xmm7
	addq	$4, %rbp
	subl	$1, %ecx
	testb	$15, %bpl
	je	.L79
	testl	%ecx, %ecx
	jne	.L80
	jmp	.L79
.L93:
	movaps	.LC3(%rip), %xmm0
	movaps	%xmm0, %xmm8
	movaps	%xmm0, %xmm5
	movaps	%xmm0, %xmm6
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm4
	movaps	%xmm0, %xmm2
	movaps	%xmm0, %xmm1
	jmp	.L83
.LFE50:
	.size	simd_v8_combine, .-simd_v8_combine
	.p2align 4,,15
.globl simd_v4_combine
	.type	simd_v4_combine, @function
simd_v4_combine:
.LFB49:
	pushq	%r12
.LCFI21:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI22:
	pushq	%rbx
.LCFI23:
	movq	%rdi, %rbx
	subq	$16, %rsp
.LCFI24:
	call	get_vec_start
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	vec_length
	testb	$15, %bpl
	movl	%eax, %ecx
	jne	.L110
.L106:
	movss	.LC2(%rip), %xmm4
	.p2align 4,,10
	.p2align 3
.L97:
	cmpl	$15, %ecx
	jle	.L111
	movl	%ecx, %edi
	movaps	.LC3(%rip), %xmm0
	leal	-16(%rdi), %esi
	movq	%rbp, %rdx
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm2
	movl	%esi, %ecx
	movaps	%xmm0, %xmm1
	shrl	$4, %ecx
	mov	%ecx, %eax
	salq	$6, %rax
	leaq	64(%rbp,%rax), %rax
	.p2align 4,,10
	.p2align 3
.L102:
	mulps	(%rdx), %xmm0
	mulps	16(%rdx), %xmm3
	mulps	32(%rdx), %xmm2
	mulps	48(%rdx), %xmm1
	addq	$64, %rdx
	cmpq	%rax, %rdx
	jne	.L102
	leal	-16(%rdi), %eax
	sall	$4, %ecx
	subl	%ecx, %esi
	shrl	$4, %eax
	movl	%esi, %ecx
	mov	%eax, %eax
	salq	$6, %rax
	leaq	64(%rbp,%rax), %rbp
.L101:
	testl	%ecx, %ecx
	je	.L103
	leal	-1(%rcx), %eax
	movq	%rbp, %rdx
	leaq	4(%rbp,%rax,4), %rax
	.p2align 4,,10
	.p2align 3
.L104:
	mulss	(%rdx), %xmm4
	addq	$4, %rdx
	cmpq	%rax, %rdx
	jne	.L104
.L103:
	mulps	%xmm3, %xmm0
	mulps	%xmm1, %xmm2
	mulps	%xmm2, %xmm0
	movaps	%xmm0, (%rsp)
	mulss	(%rsp), %xmm4
	mulss	4(%rsp), %xmm4
	mulss	8(%rsp), %xmm4
	mulss	12(%rsp), %xmm4
	movss	%xmm4, (%r12)
	addq	$16, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L110:
	testl	%eax, %eax
	je	.L106
	movss	.LC2(%rip), %xmm4
.L98:
	mulss	(%rbp), %xmm4
	addq	$4, %rbp
	subl	$1, %ecx
	testb	$15, %bpl
	je	.L97
	testl	%ecx, %ecx
	jne	.L98
	jmp	.L97
.L111:
	movaps	.LC3(%rip), %xmm0
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm2
	movaps	%xmm0, %xmm1
	jmp	.L101
.LFE49:
	.size	simd_v4_combine, .-simd_v4_combine
	.p2align 4,,15
.globl simd_v2_combine
	.type	simd_v2_combine, @function
simd_v2_combine:
.LFB48:
	pushq	%r12
.LCFI25:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI26:
	pushq	%rbx
.LCFI27:
	movq	%rdi, %rbx
	subq	$16, %rsp
.LCFI28:
	call	get_vec_start
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	vec_length
	testb	$15, %bpl
	movl	%eax, %ecx
	jne	.L128
.L124:
	movss	.LC2(%rip), %xmm2
	.p2align 4,,10
	.p2align 3
.L115:
	cmpl	$7, %ecx
	jle	.L129
	movl	%ecx, %edi
	movaps	.LC3(%rip), %xmm0
	leal	-8(%rdi), %esi
	movq	%rbp, %rdx
	movaps	%xmm0, %xmm1
	movl	%esi, %ecx
	shrl	$3, %ecx
	mov	%ecx, %eax
	salq	$5, %rax
	leaq	32(%rbp,%rax), %rax
	.p2align 4,,10
	.p2align 3
.L120:
	mulps	(%rdx), %xmm0
	mulps	16(%rdx), %xmm1
	addq	$32, %rdx
	cmpq	%rax, %rdx
	jne	.L120
	leal	0(,%rcx,8), %eax
	movl	%esi, %ecx
	subl	%eax, %ecx
	leal	-8(%rdi), %eax
	shrl	$3, %eax
	mov	%eax, %eax
	salq	$5, %rax
	leaq	32(%rbp,%rax), %rbp
.L119:
	testl	%ecx, %ecx
	je	.L121
	leal	-1(%rcx), %eax
	movq	%rbp, %rdx
	leaq	4(%rbp,%rax,4), %rax
	.p2align 4,,10
	.p2align 3
.L122:
	mulss	(%rdx), %xmm2
	addq	$4, %rdx
	cmpq	%rax, %rdx
	jne	.L122
.L121:
	mulps	%xmm1, %xmm0
	movaps	%xmm0, (%rsp)
	mulss	(%rsp), %xmm2
	mulss	4(%rsp), %xmm2
	mulss	8(%rsp), %xmm2
	mulss	12(%rsp), %xmm2
	movss	%xmm2, (%r12)
	addq	$16, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L128:
	testl	%eax, %eax
	je	.L124
	movss	.LC2(%rip), %xmm2
.L116:
	mulss	(%rbp), %xmm2
	addq	$4, %rbp
	subl	$1, %ecx
	testb	$15, %bpl
	je	.L115
	testl	%ecx, %ecx
	jne	.L116
	jmp	.L115
.L129:
	movaps	.LC3(%rip), %xmm0
	movaps	%xmm0, %xmm1
	.p2align 4,,2
	.p2align 3
	jmp	.L119
.LFE48:
	.size	simd_v2_combine, .-simd_v2_combine
	.p2align 4,,15
.globl simd_v1_combine
	.type	simd_v1_combine, @function
simd_v1_combine:
.LFB47:
	pushq	%r12
.LCFI29:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI30:
	pushq	%rbx
.LCFI31:
	movq	%rdi, %rbx
	subq	$16, %rsp
.LCFI32:
	call	get_vec_start
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	vec_length
	testb	$15, %bpl
	movl	%eax, %ecx
	jne	.L146
.L142:
	movss	.LC2(%rip), %xmm1
	.p2align 4,,10
	.p2align 3
.L133:
	cmpl	$3, %ecx
	jle	.L147
	movl	%ecx, %edi
	movaps	.LC3(%rip), %xmm0
	leal	-4(%rdi), %esi
	movq	%rbp, %rdx
	movl	%esi, %ecx
	shrl	$2, %ecx
	mov	%ecx, %eax
	salq	$4, %rax
	leaq	16(%rbp,%rax), %rax
	.p2align 4,,10
	.p2align 3
.L138:
	mulps	(%rdx), %xmm0
	addq	$16, %rdx
	cmpq	%rax, %rdx
	jne	.L138
	leal	0(,%rcx,4), %eax
	movl	%esi, %ecx
	subl	%eax, %ecx
	leal	-4(%rdi), %eax
	shrl	$2, %eax
	mov	%eax, %eax
	salq	$4, %rax
	leaq	16(%rbp,%rax), %rbp
.L137:
	testl	%ecx, %ecx
	je	.L139
	leal	-1(%rcx), %eax
	movq	%rbp, %rdx
	leaq	4(%rbp,%rax,4), %rax
	.p2align 4,,10
	.p2align 3
.L140:
	mulss	(%rdx), %xmm1
	addq	$4, %rdx
	cmpq	%rax, %rdx
	jne	.L140
.L139:
	movaps	%xmm0, (%rsp)
	mulss	(%rsp), %xmm1
	mulss	4(%rsp), %xmm1
	mulss	8(%rsp), %xmm1
	mulss	12(%rsp), %xmm1
	movss	%xmm1, (%r12)
	addq	$16, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L146:
	testl	%eax, %eax
	je	.L142
	movss	.LC2(%rip), %xmm1
.L134:
	mulss	(%rbp), %xmm1
	addq	$4, %rbp
	subl	$1, %ecx
	testb	$15, %bpl
	je	.L133
	testl	%ecx, %ecx
	jne	.L134
	jmp	.L133
.L147:
	movaps	.LC3(%rip), %xmm0
	.p2align 4,,4
	.p2align 3
	jmp	.L137
.LFE47:
	.size	simd_v1_combine, .-simd_v1_combine
	.p2align 4,,15
.globl unroll8aa_combine
	.type	unroll8aa_combine, @function
unroll8aa_combine:
.LFB46:
	pushq	%r13
.LCFI33:
	movq	%rsi, %r13
	pushq	%r12
.LCFI34:
	pushq	%rbp
.LCFI35:
	pushq	%rbx
.LCFI36:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI37:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-7(%rbp), %r12d
	call	get_vec_start
	movq	%rax, %rcx
	movslq	%r12d,%rax
	testq	%rax, %rax
	jle	.L157
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm3
	.p2align 4,,10
	.p2align 3
.L151:
	movss	(%rcx,%rdx,4), %xmm2
	movss	8(%rcx,%rdx,4), %xmm0
	mulss	4(%rcx,%rdx,4), %xmm2
	mulss	12(%rcx,%rdx,4), %xmm0
	movss	16(%rcx,%rdx,4), %xmm1
	mulss	20(%rcx,%rdx,4), %xmm1
	mulss	%xmm0, %xmm2
	movss	24(%rcx,%rdx,4), %xmm0
	mulss	28(%rcx,%rdx,4), %xmm0
	addq	$8, %rdx
	cmpq	%rax, %rdx
	mulss	%xmm0, %xmm1
	mulss	%xmm1, %xmm2
	mulss	%xmm2, %xmm3
	jl	.L151
	subq	$1, %rax
	shrq	$3, %rax
	leaq	8(,%rax,8), %rdx
.L150:
	movslq	%ebp,%r12
	cmpq	%r12, %rdx
	jge	.L152
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L153:
	addq	$1, %rdx
	mulss	(%rax), %xmm3
	addq	$4, %rax
	cmpq	%rdx, %r12
	jg	.L153
.L152:
	movss	%xmm3, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L157:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm3
	jmp	.L150
.LFE46:
	.size	unroll8aa_combine, .-unroll8aa_combine
	.p2align 4,,15
.globl unroll6aa_combine
	.type	unroll6aa_combine, @function
unroll6aa_combine:
.LFB45:
	pushq	%r13
.LCFI38:
	movq	%rsi, %r13
	pushq	%r12
.LCFI39:
	pushq	%rbp
.LCFI40:
	pushq	%rbx
.LCFI41:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI42:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-5(%rbp), %r12d
	call	get_vec_start
	movslq	%r12d,%rdx
	movq	%rax, %rcx
	testq	%rdx, %rdx
	jle	.L167
	xorl	%eax, %eax
	movss	.LC2(%rip), %xmm2
	.p2align 4,,10
	.p2align 3
.L161:
	movss	(%rcx,%rax,4), %xmm0
	movss	8(%rcx,%rax,4), %xmm1
	mulss	4(%rcx,%rax,4), %xmm0
	mulss	12(%rcx,%rax,4), %xmm1
	mulss	%xmm1, %xmm0
	movss	16(%rcx,%rax,4), %xmm1
	mulss	20(%rcx,%rax,4), %xmm1
	addq	$6, %rax
	cmpq	%rdx, %rax
	mulss	%xmm1, %xmm0
	mulss	%xmm0, %xmm2
	jl	.L161
	subq	$1, %rdx
	movabsq	$-6148914691236517205, %rax
	mulq	%rdx
	shrq	$2, %rdx
	leaq	3(%rdx,%rdx,2), %rdx
	addq	%rdx, %rdx
.L160:
	movslq	%ebp,%r12
	cmpq	%r12, %rdx
	jge	.L162
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L163:
	addq	$1, %rdx
	mulss	(%rax), %xmm2
	addq	$4, %rax
	cmpq	%rdx, %r12
	jg	.L163
.L162:
	movss	%xmm2, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L167:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm2
	jmp	.L160
.LFE45:
	.size	unroll6aa_combine, .-unroll6aa_combine
	.p2align 4,,15
.globl unroll5aa_combine
	.type	unroll5aa_combine, @function
unroll5aa_combine:
.LFB44:
	pushq	%r13
.LCFI43:
	movq	%rsi, %r13
	pushq	%r12
.LCFI44:
	pushq	%rbp
.LCFI45:
	pushq	%rbx
.LCFI46:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI47:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-4(%rbp), %r12d
	call	get_vec_start
	movslq	%r12d,%rdx
	movq	%rax, %rcx
	testq	%rdx, %rdx
	jle	.L177
	xorl	%eax, %eax
	movss	.LC2(%rip), %xmm2
	.p2align 4,,10
	.p2align 3
.L171:
	movss	(%rcx,%rax,4), %xmm0
	movss	8(%rcx,%rax,4), %xmm1
	mulss	4(%rcx,%rax,4), %xmm0
	mulss	12(%rcx,%rax,4), %xmm1
	mulss	%xmm1, %xmm0
	mulss	16(%rcx,%rax,4), %xmm0
	addq	$5, %rax
	cmpq	%rdx, %rax
	mulss	%xmm0, %xmm2
	jl	.L171
	subq	$1, %rdx
	movabsq	$-3689348814741910323, %rax
	mulq	%rdx
	shrq	$2, %rdx
	leaq	5(%rdx,%rdx,4), %rdx
.L170:
	movslq	%ebp,%r12
	cmpq	%r12, %rdx
	jge	.L172
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L173:
	addq	$1, %rdx
	mulss	(%rax), %xmm2
	addq	$4, %rax
	cmpq	%rdx, %r12
	jg	.L173
.L172:
	movss	%xmm2, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L177:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm2
	jmp	.L170
.LFE44:
	.size	unroll5aa_combine, .-unroll5aa_combine
	.p2align 4,,15
.globl unroll4aa_combine
	.type	unroll4aa_combine, @function
unroll4aa_combine:
.LFB43:
	pushq	%r13
.LCFI48:
	movq	%rsi, %r13
	pushq	%r12
.LCFI49:
	pushq	%rbp
.LCFI50:
	pushq	%rbx
.LCFI51:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI52:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-3(%rbp), %r12d
	call	get_vec_start
	movq	%rax, %rcx
	movslq	%r12d,%rax
	testq	%rax, %rax
	jle	.L187
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm2
	.p2align 4,,10
	.p2align 3
.L181:
	movss	(%rcx,%rdx,4), %xmm1
	movss	8(%rcx,%rdx,4), %xmm0
	mulss	4(%rcx,%rdx,4), %xmm1
	mulss	12(%rcx,%rdx,4), %xmm0
	addq	$4, %rdx
	cmpq	%rax, %rdx
	mulss	%xmm0, %xmm1
	mulss	%xmm1, %xmm2
	jl	.L181
	subq	$1, %rax
	shrq	$2, %rax
	leaq	4(,%rax,4), %rdx
.L180:
	movslq	%ebp,%r12
	cmpq	%r12, %rdx
	jge	.L182
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L183:
	addq	$1, %rdx
	mulss	(%rax), %xmm2
	addq	$4, %rax
	cmpq	%r12, %rdx
	jl	.L183
.L182:
	movss	%xmm2, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L187:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm2
	jmp	.L180
.LFE43:
	.size	unroll4aa_combine, .-unroll4aa_combine
	.p2align 4,,15
.globl unroll3aa_combine
	.type	unroll3aa_combine, @function
unroll3aa_combine:
.LFB42:
	pushq	%r13
.LCFI53:
	movq	%rsi, %r13
	pushq	%r12
.LCFI54:
	pushq	%rbp
.LCFI55:
	pushq	%rbx
.LCFI56:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI57:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-2(%rbp), %r12d
	call	get_vec_start
	movslq	%r12d,%rdx
	movq	%rax, %rcx
	testq	%rdx, %rdx
	jle	.L197
	xorl	%eax, %eax
	movss	.LC2(%rip), %xmm1
	.p2align 4,,10
	.p2align 3
.L191:
	movss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm0
	mulss	8(%rcx,%rax,4), %xmm0
	addq	$3, %rax
	cmpq	%rdx, %rax
	mulss	%xmm0, %xmm1
	jl	.L191
	subq	$1, %rdx
	movabsq	$-6148914691236517205, %rax
	mulq	%rdx
	shrq	%rdx
	leaq	3(%rdx,%rdx,2), %rdx
.L190:
	movslq	%ebp,%r12
	cmpq	%r12, %rdx
	jge	.L192
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L193:
	addq	$1, %rdx
	mulss	(%rax), %xmm1
	addq	$4, %rax
	cmpq	%r12, %rdx
	jl	.L193
.L192:
	movss	%xmm1, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L197:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm1
	jmp	.L190
.LFE42:
	.size	unroll3aa_combine, .-unroll3aa_combine
	.p2align 4,,15
.globl combine7
	.type	combine7, @function
combine7:
.LFB41:
	pushq	%r13
.LCFI58:
	movq	%rsi, %r13
	pushq	%r12
.LCFI59:
	pushq	%rbp
.LCFI60:
	pushq	%rbx
.LCFI61:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI62:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-1(%rbp), %r12d
	call	get_vec_start
	movq	%rax, %rcx
	movslq	%r12d,%rax
	testq	%rax, %rax
	jle	.L207
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm1
	.p2align 4,,10
	.p2align 3
.L201:
	movss	(%rcx,%rdx,4), %xmm0
	mulss	4(%rcx,%rdx,4), %xmm0
	addq	$2, %rdx
	cmpq	%rax, %rdx
	mulss	%xmm0, %xmm1
	jl	.L201
	subq	$1, %rax
	shrq	%rax
	leaq	2(%rax,%rax), %rdx
.L200:
	movslq	%ebp,%r12
	cmpq	%r12, %rdx
	jge	.L202
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L203:
	addq	$1, %rdx
	mulss	(%rax), %xmm1
	addq	$4, %rax
	cmpq	%r12, %rdx
	jl	.L203
.L202:
	movss	%xmm1, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L207:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm1
	jmp	.L200
.LFE41:
	.size	combine7, .-combine7
	.p2align 4,,15
.globl unroll8x8_combine
	.type	unroll8x8_combine, @function
unroll8x8_combine:
.LFB40:
	pushq	%r12
.LCFI63:
	movq	%rdi, %r12
	pushq	%rbp
.LCFI64:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI65:
	call	vec_length
	movl	%eax, %ebx
	movq	%r12, %rdi
	movslq	%ebx,%rbx
	call	get_vec_start
	leaq	-28(%rax,%rbx,4), %rcx
	movq	%rax, %rdx
	cmpq	%rcx, %rax
	jae	.L218
	movss	.LC2(%rip), %xmm0
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm7
	movaps	%xmm0, %xmm6
	movaps	%xmm0, %xmm5
	movaps	%xmm0, %xmm4
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm2
	.p2align 4,,10
	.p2align 3
.L211:
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm1
	mulss	8(%rax), %xmm7
	mulss	12(%rax), %xmm6
	mulss	16(%rax), %xmm5
	mulss	20(%rax), %xmm4
	mulss	24(%rax), %xmm3
	mulss	28(%rax), %xmm2
	addq	$32, %rax
	cmpq	%rax, %rcx
	ja	.L211
	movq	%rdx, %rax
	notq	%rax
	addq	%rcx, %rax
	andq	$-32, %rax
	leaq	32(%rdx,%rax), %rdx
.L210:
	leaq	28(%rcx), %rax
	cmpq	%rdx, %rax
	jbe	.L212
	.p2align 4,,10
	.p2align 3
.L215:
	mulss	(%rdx), %xmm0
	addq	$4, %rdx
	cmpq	%rdx, %rax
	ja	.L215
.L212:
	mulss	%xmm1, %xmm0
	mulss	%xmm7, %xmm0
	mulss	%xmm6, %xmm0
	mulss	%xmm5, %xmm0
	mulss	%xmm4, %xmm0
	mulss	%xmm3, %xmm0
	mulss	%xmm2, %xmm0
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L218:
	movss	.LC2(%rip), %xmm1
	movaps	%xmm1, %xmm7
	movaps	%xmm1, %xmm6
	movaps	%xmm1, %xmm5
	movaps	%xmm1, %xmm4
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm2
	movaps	%xmm1, %xmm0
	jmp	.L210
.LFE40:
	.size	unroll8x8_combine, .-unroll8x8_combine
	.p2align 4,,15
.globl unroll8x4_combine
	.type	unroll8x4_combine, @function
unroll8x4_combine:
.LFB39:
	pushq	%r12
.LCFI66:
	movq	%rdi, %r12
	pushq	%rbp
.LCFI67:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI68:
	call	vec_length
	movl	%eax, %ebx
	movq	%r12, %rdi
	movslq	%ebx,%rbx
	call	get_vec_start
	leaq	-28(%rax,%rbx,4), %rcx
	movq	%rax, %rdx
	cmpq	%rcx, %rax
	jae	.L229
	movss	.LC2(%rip), %xmm1
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm0
	movaps	%xmm1, %xmm2
	.p2align 4,,10
	.p2align 3
.L222:
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm2
	mulss	8(%rax), %xmm1
	mulss	12(%rax), %xmm3
	mulss	16(%rax), %xmm0
	mulss	20(%rax), %xmm2
	mulss	24(%rax), %xmm1
	mulss	28(%rax), %xmm3
	addq	$32, %rax
	cmpq	%rax, %rcx
	ja	.L222
	movq	%rdx, %rax
	notq	%rax
	addq	%rcx, %rax
	andq	$-32, %rax
	leaq	32(%rdx,%rax), %rdx
.L221:
	leaq	28(%rcx), %rax
	cmpq	%rdx, %rax
	jbe	.L223
	.p2align 4,,10
	.p2align 3
.L226:
	mulss	(%rdx), %xmm0
	addq	$4, %rdx
	cmpq	%rdx, %rax
	ja	.L226
.L223:
	mulss	%xmm2, %xmm0
	mulss	%xmm1, %xmm0
	mulss	%xmm3, %xmm0
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L229:
	movss	.LC2(%rip), %xmm1
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm2
	movaps	%xmm1, %xmm0
	jmp	.L221
.LFE39:
	.size	unroll8x4_combine, .-unroll8x4_combine
	.p2align 4,,15
.globl unroll9x3_combine
	.type	unroll9x3_combine, @function
unroll9x3_combine:
.LFB38:
	pushq	%r12
.LCFI69:
	movq	%rdi, %r12
	pushq	%rbp
.LCFI70:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI71:
	call	vec_length
	movl	%eax, %ebx
	movq	%r12, %rdi
	movslq	%ebx,%rbx
	call	get_vec_start
	leaq	-32(%rax,%rbx,4), %rsi
	movq	%rax, %rcx
	cmpq	%rsi, %rax
	jae	.L240
	movss	.LC2(%rip), %xmm0
	movaps	%xmm0, %xmm2
	movaps	%xmm0, %xmm1
	.p2align 4,,10
	.p2align 3
.L233:
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm2
	mulss	8(%rax), %xmm1
	mulss	12(%rax), %xmm0
	mulss	16(%rax), %xmm2
	mulss	20(%rax), %xmm1
	mulss	24(%rax), %xmm0
	mulss	28(%rax), %xmm2
	mulss	32(%rax), %xmm1
	addq	$36, %rax
	cmpq	%rax, %rsi
	ja	.L233
	movq	%rcx, %rdx
	movabsq	$-2049638230412172401, %rax
	notq	%rdx
	addq	%rsi, %rdx
	mulq	%rdx
	shrq	$5, %rdx
	leaq	9(%rdx,%rdx,8), %rdx
	leaq	(%rcx,%rdx,4), %rcx
.L232:
	leaq	32(%rsi), %rax
	cmpq	%rcx, %rax
	jbe	.L234
	.p2align 4,,10
	.p2align 3
.L237:
	mulss	(%rcx), %xmm0
	addq	$4, %rcx
	cmpq	%rcx, %rax
	ja	.L237
.L234:
	mulss	%xmm2, %xmm0
	mulss	%xmm1, %xmm0
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L240:
	movss	.LC2(%rip), %xmm2
	movaps	%xmm2, %xmm1
	movaps	%xmm2, %xmm0
	jmp	.L232
.LFE38:
	.size	unroll9x3_combine, .-unroll9x3_combine
	.p2align 4,,15
.globl unroll8x2_combine
	.type	unroll8x2_combine, @function
unroll8x2_combine:
.LFB37:
	pushq	%r12
.LCFI72:
	movq	%rdi, %r12
	pushq	%rbp
.LCFI73:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI74:
	call	vec_length
	movl	%eax, %ebx
	movq	%r12, %rdi
	movslq	%ebx,%rbx
	call	get_vec_start
	leaq	-28(%rax,%rbx,4), %rcx
	movq	%rax, %rdx
	cmpq	%rcx, %rax
	jae	.L251
	movss	.LC2(%rip), %xmm0
	movaps	%xmm0, %xmm1
	.p2align 4,,10
	.p2align 3
.L244:
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm1
	mulss	8(%rax), %xmm0
	mulss	12(%rax), %xmm1
	mulss	16(%rax), %xmm0
	mulss	20(%rax), %xmm1
	mulss	24(%rax), %xmm0
	mulss	28(%rax), %xmm1
	addq	$32, %rax
	cmpq	%rax, %rcx
	ja	.L244
	movq	%rdx, %rax
	notq	%rax
	addq	%rcx, %rax
	andq	$-32, %rax
	leaq	32(%rdx,%rax), %rdx
.L243:
	leaq	28(%rcx), %rax
	cmpq	%rdx, %rax
	jbe	.L245
	.p2align 4,,10
	.p2align 3
.L248:
	mulss	(%rdx), %xmm0
	addq	$4, %rdx
	cmpq	%rdx, %rax
	ja	.L248
.L245:
	mulss	%xmm1, %xmm0
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L251:
	movss	.LC2(%rip), %xmm1
	movaps	%xmm1, %xmm0
	jmp	.L243
.LFE37:
	.size	unroll8x2_combine, .-unroll8x2_combine
	.p2align 4,,15
.globl unroll4x2as_combine
	.type	unroll4x2as_combine, @function
unroll4x2as_combine:
.LFB36:
	pushq	%r13
.LCFI75:
	movq	%rsi, %r13
	pushq	%r12
.LCFI76:
	pushq	%rbp
.LCFI77:
	pushq	%rbx
.LCFI78:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI79:
	call	vec_length
	movl	%eax, %r12d
	shrl	$31, %eax
	movq	%rbx, %rdi
	leal	(%rax,%r12), %ebp
	call	get_vec_start
	sarl	%ebp
	movq	%rax, %rcx
	movslq	%ebp,%rdx
	testq	%rdx, %rdx
	leaq	(%rax,%rdx,4), %rsi
	jle	.L261
	movss	.LC2(%rip), %xmm1
	xorl	%eax, %eax
	movaps	%xmm1, %xmm0
	.p2align 4,,10
	.p2align 3
.L255:
	mulss	(%rcx,%rax,4), %xmm1
	mulss	(%rsi,%rax,4), %xmm0
	addq	$1, %rax
	cmpq	%rdx, %rax
	jl	.L255
.L254:
	leal	(%rbp,%rbp), %eax
	movslq	%r12d,%rdi
	movslq	%eax,%rsi
	cmpq	%rdi, %rsi
	jge	.L256
	leaq	(%rcx,%rsi,4), %rcx
	xorl	%edx, %edx
	.p2align 4,,10
	.p2align 3
.L257:
	addq	$1, %rdx
	mulss	(%rcx), %xmm0
	addq	$4, %rcx
	leaq	(%rsi,%rdx), %rax
	cmpq	%rax, %rdi
	jg	.L257
.L256:
	mulss	%xmm0, %xmm1
	movss	%xmm1, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L261:
	movss	.LC2(%rip), %xmm1
	movaps	%xmm1, %xmm0
	jmp	.L254
.LFE36:
	.size	unroll4x2as_combine, .-unroll4x2as_combine
	.p2align 4,,15
.globl unrollx2as_combine
	.type	unrollx2as_combine, @function
unrollx2as_combine:
.LFB35:
	pushq	%r13
.LCFI80:
	movq	%rsi, %r13
	pushq	%r12
.LCFI81:
	pushq	%rbp
.LCFI82:
	pushq	%rbx
.LCFI83:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI84:
	call	vec_length
	movl	%eax, %r12d
	shrl	$31, %eax
	movq	%rbx, %rdi
	leal	(%rax,%r12), %ebp
	call	get_vec_start
	sarl	%ebp
	movq	%rax, %rcx
	movslq	%ebp,%rdx
	testq	%rdx, %rdx
	leaq	(%rax,%rdx,4), %rsi
	jle	.L271
	movss	.LC2(%rip), %xmm1
	xorl	%eax, %eax
	movaps	%xmm1, %xmm0
	.p2align 4,,10
	.p2align 3
.L265:
	mulss	(%rcx,%rax,4), %xmm1
	mulss	(%rsi,%rax,4), %xmm0
	addq	$1, %rax
	cmpq	%rdx, %rax
	jl	.L265
.L264:
	leal	(%rbp,%rbp), %eax
	movslq	%r12d,%rdi
	movslq	%eax,%rsi
	cmpq	%rdi, %rsi
	jge	.L266
	leaq	(%rcx,%rsi,4), %rcx
	xorl	%edx, %edx
	.p2align 4,,10
	.p2align 3
.L267:
	addq	$1, %rdx
	mulss	(%rcx), %xmm0
	addq	$4, %rcx
	leaq	(%rsi,%rdx), %rax
	cmpq	%rax, %rdi
	jg	.L267
.L266:
	mulss	%xmm0, %xmm1
	movss	%xmm1, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L271:
	movss	.LC2(%rip), %xmm1
	movaps	%xmm1, %xmm0
	jmp	.L264
.LFE35:
	.size	unrollx2as_combine, .-unrollx2as_combine
	.p2align 4,,15
.globl unroll10x10a_combine
	.type	unroll10x10a_combine, @function
unroll10x10a_combine:
.LFB34:
	pushq	%r13
.LCFI85:
	movq	%rsi, %r13
	pushq	%r12
.LCFI86:
	pushq	%rbp
.LCFI87:
	pushq	%rbx
.LCFI88:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI89:
	call	vec_length
	movl	%eax, %r12d
	movq	%rbx, %rdi
	leal	-9(%r12), %ebp
	call	get_vec_start
	testl	%ebp, %ebp
	movq	%rax, %rdi
	jle	.L281
	movslq	%ebp,%rdx
	movq	%rax, %rcx
	movabsq	$-3689348814741910323, %rax
	subq	$1, %rdx
	movss	.LC2(%rip), %xmm0
	mulq	%rdx
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm9
	xorl	%esi, %esi
	movaps	%xmm0, %xmm8
	shrq	$3, %rdx
	movaps	%xmm0, %xmm7
	leaq	5(%rdx,%rdx,4), %rdx
	movaps	%xmm0, %xmm6
	movaps	%xmm0, %xmm5
	movaps	%xmm0, %xmm4
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm2
	leaq	0(,%rdx,8), %rax
	.p2align 4,,10
	.p2align 3
.L275:
	addq	$40, %rsi
	mulss	(%rcx), %xmm0
	mulss	4(%rcx), %xmm1
	mulss	8(%rcx), %xmm9
	mulss	12(%rcx), %xmm8
	mulss	16(%rcx), %xmm7
	mulss	20(%rcx), %xmm6
	mulss	24(%rcx), %xmm5
	mulss	28(%rcx), %xmm4
	mulss	32(%rcx), %xmm3
	mulss	36(%rcx), %xmm2
	addq	$40, %rcx
	cmpq	%rax, %rsi
	jne	.L275
	addq	%rdx, %rdx
.L274:
	movslq	%r12d,%rcx
	cmpq	%rdx, %rcx
	jle	.L276
	leaq	(%rdi,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L277:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rcx
	jg	.L277
.L276:
	mulss	%xmm1, %xmm0
	mulss	%xmm9, %xmm0
	mulss	%xmm8, %xmm0
	mulss	%xmm7, %xmm0
	mulss	%xmm6, %xmm0
	mulss	%xmm5, %xmm0
	mulss	%xmm4, %xmm0
	mulss	%xmm3, %xmm0
	mulss	%xmm2, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L281:
	movss	.LC2(%rip), %xmm1
	xorl	%edx, %edx
	movaps	%xmm1, %xmm9
	movaps	%xmm1, %xmm8
	movaps	%xmm1, %xmm7
	movaps	%xmm1, %xmm6
	movaps	%xmm1, %xmm5
	movaps	%xmm1, %xmm4
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm2
	movaps	%xmm1, %xmm0
	jmp	.L274
.LFE34:
	.size	unroll10x10a_combine, .-unroll10x10a_combine
	.p2align 4,,15
.globl unroll8x8a_combine
	.type	unroll8x8a_combine, @function
unroll8x8a_combine:
.LFB33:
	pushq	%r13
.LCFI90:
	movq	%rsi, %r13
	pushq	%r12
.LCFI91:
	pushq	%rbp
.LCFI92:
	pushq	%rbx
.LCFI93:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI94:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-7(%rbp), %r12d
	call	get_vec_start
	movq	%rax, %rcx
	movslq	%r12d,%rax
	testq	%rax, %rax
	jle	.L291
	movss	.LC2(%rip), %xmm0
	xorl	%edx, %edx
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm7
	movaps	%xmm0, %xmm6
	movaps	%xmm0, %xmm5
	movaps	%xmm0, %xmm4
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm2
	.p2align 4,,10
	.p2align 3
.L285:
	mulss	(%rcx,%rdx,4), %xmm0
	mulss	4(%rcx,%rdx,4), %xmm1
	mulss	8(%rcx,%rdx,4), %xmm7
	mulss	12(%rcx,%rdx,4), %xmm6
	mulss	16(%rcx,%rdx,4), %xmm5
	mulss	20(%rcx,%rdx,4), %xmm4
	mulss	24(%rcx,%rdx,4), %xmm3
	mulss	28(%rcx,%rdx,4), %xmm2
	addq	$8, %rdx
	cmpq	%rax, %rdx
	jl	.L285
	subq	$1, %rax
	shrq	$3, %rax
	leaq	8(,%rax,8), %rdx
.L284:
	movslq	%ebp,%r12
	cmpq	%rdx, %r12
	jle	.L286
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L287:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %r12
	jg	.L287
.L286:
	mulss	%xmm1, %xmm0
	mulss	%xmm7, %xmm0
	mulss	%xmm6, %xmm0
	mulss	%xmm5, %xmm0
	mulss	%xmm4, %xmm0
	mulss	%xmm3, %xmm0
	mulss	%xmm2, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L291:
	movss	.LC2(%rip), %xmm1
	xorl	%edx, %edx
	movaps	%xmm1, %xmm7
	movaps	%xmm1, %xmm6
	movaps	%xmm1, %xmm5
	movaps	%xmm1, %xmm4
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm2
	movaps	%xmm1, %xmm0
	jmp	.L284
.LFE33:
	.size	unroll8x8a_combine, .-unroll8x8a_combine
	.p2align 4,,15
.globl unroll6x6a_combine
	.type	unroll6x6a_combine, @function
unroll6x6a_combine:
.LFB32:
	pushq	%r13
.LCFI95:
	movq	%rsi, %r13
	pushq	%r12
.LCFI96:
	pushq	%rbp
.LCFI97:
	pushq	%rbx
.LCFI98:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI99:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-5(%rbp), %r12d
	call	get_vec_start
	movslq	%r12d,%rdx
	movq	%rax, %rcx
	testq	%rdx, %rdx
	jle	.L301
	movss	.LC2(%rip), %xmm0
	xorl	%eax, %eax
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm5
	movaps	%xmm0, %xmm4
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm2
	.p2align 4,,10
	.p2align 3
.L295:
	mulss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm1
	mulss	8(%rcx,%rax,4), %xmm5
	mulss	12(%rcx,%rax,4), %xmm4
	mulss	16(%rcx,%rax,4), %xmm3
	mulss	20(%rcx,%rax,4), %xmm2
	addq	$6, %rax
	cmpq	%rdx, %rax
	jl	.L295
	subq	$1, %rdx
	movabsq	$-6148914691236517205, %rax
	mulq	%rdx
	shrq	$2, %rdx
	leaq	3(%rdx,%rdx,2), %rdx
	addq	%rdx, %rdx
.L294:
	movslq	%ebp,%r12
	cmpq	%rdx, %r12
	jle	.L296
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L297:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %r12
	jg	.L297
.L296:
	mulss	%xmm1, %xmm0
	mulss	%xmm5, %xmm0
	mulss	%xmm4, %xmm0
	mulss	%xmm3, %xmm0
	mulss	%xmm2, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L301:
	movss	.LC2(%rip), %xmm1
	xorl	%edx, %edx
	movaps	%xmm1, %xmm5
	movaps	%xmm1, %xmm4
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm2
	movaps	%xmm1, %xmm0
	jmp	.L294
.LFE32:
	.size	unroll6x6a_combine, .-unroll6x6a_combine
	.p2align 4,,15
.globl unroll5x5a_combine
	.type	unroll5x5a_combine, @function
unroll5x5a_combine:
.LFB31:
	pushq	%r13
.LCFI100:
	movq	%rsi, %r13
	pushq	%r12
.LCFI101:
	pushq	%rbp
.LCFI102:
	pushq	%rbx
.LCFI103:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI104:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-4(%rbp), %r12d
	call	get_vec_start
	movslq	%r12d,%rdx
	movq	%rax, %rcx
	testq	%rdx, %rdx
	jle	.L311
	movss	.LC2(%rip), %xmm0
	xorl	%eax, %eax
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm4
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm2
	.p2align 4,,10
	.p2align 3
.L305:
	mulss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm1
	mulss	8(%rcx,%rax,4), %xmm4
	mulss	12(%rcx,%rax,4), %xmm3
	mulss	16(%rcx,%rax,4), %xmm2
	addq	$5, %rax
	cmpq	%rdx, %rax
	jl	.L305
	subq	$1, %rdx
	movabsq	$-3689348814741910323, %rax
	mulq	%rdx
	shrq	$2, %rdx
	leaq	5(%rdx,%rdx,4), %rdx
.L304:
	movslq	%ebp,%r12
	cmpq	%rdx, %r12
	jle	.L306
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L307:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %r12
	jg	.L307
.L306:
	mulss	%xmm1, %xmm0
	mulss	%xmm4, %xmm0
	mulss	%xmm3, %xmm0
	mulss	%xmm2, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L311:
	movss	.LC2(%rip), %xmm1
	xorl	%edx, %edx
	movaps	%xmm1, %xmm4
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm2
	movaps	%xmm1, %xmm0
	jmp	.L304
.LFE31:
	.size	unroll5x5a_combine, .-unroll5x5a_combine
	.p2align 4,,15
.globl unroll12x12a_combine
	.type	unroll12x12a_combine, @function
unroll12x12a_combine:
.LFB30:
	pushq	%r13
.LCFI105:
	movq	%rsi, %r13
	pushq	%r12
.LCFI106:
	pushq	%rbp
.LCFI107:
	pushq	%rbx
.LCFI108:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI109:
	call	vec_length
	movl	%eax, %r12d
	movq	%rbx, %rdi
	leal	-11(%r12), %ebp
	call	get_vec_start
	testl	%ebp, %ebp
	movq	%rax, %rdi
	jle	.L321
	movslq	%ebp,%rdx
	movq	%rax, %rcx
	movabsq	$-6148914691236517205, %rax
	subq	$1, %rdx
	movss	.LC2(%rip), %xmm0
	mulq	%rdx
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm10
	xorl	%esi, %esi
	movaps	%xmm0, %xmm11
	shrq	$3, %rdx
	movaps	%xmm0, %xmm8
	leaq	3(%rdx,%rdx,2), %rdx
	movaps	%xmm0, %xmm9
	movaps	%xmm0, %xmm6
	movq	%rdx, %rax
	movaps	%xmm0, %xmm7
	movaps	%xmm0, %xmm4
	salq	$4, %rax
	movaps	%xmm0, %xmm5
	movaps	%xmm0, %xmm2
	movaps	%xmm0, %xmm3
	.p2align 4,,10
	.p2align 3
.L315:
	addq	$48, %rsi
	mulss	(%rcx), %xmm0
	mulss	24(%rcx), %xmm6
	mulss	4(%rcx), %xmm1
	mulss	28(%rcx), %xmm7
	mulss	8(%rcx), %xmm10
	mulss	32(%rcx), %xmm4
	mulss	12(%rcx), %xmm11
	mulss	36(%rcx), %xmm5
	mulss	16(%rcx), %xmm8
	mulss	40(%rcx), %xmm2
	mulss	20(%rcx), %xmm9
	mulss	44(%rcx), %xmm3
	addq	$48, %rcx
	cmpq	%rax, %rsi
	jne	.L315
	salq	$2, %rdx
.L314:
	movslq	%r12d,%rcx
	cmpq	%rdx, %rcx
	jle	.L316
	leaq	(%rdi,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L317:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rcx
	jg	.L317
.L316:
	mulss	%xmm1, %xmm0
	mulss	%xmm11, %xmm10
	mulss	%xmm9, %xmm8
	mulss	%xmm7, %xmm6
	mulss	%xmm5, %xmm4
	mulss	%xmm10, %xmm0
	mulss	%xmm3, %xmm2
	mulss	%xmm8, %xmm0
	mulss	%xmm6, %xmm0
	mulss	%xmm4, %xmm0
	mulss	%xmm2, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L321:
	movss	.LC2(%rip), %xmm1
	xorl	%edx, %edx
	movaps	%xmm1, %xmm10
	movaps	%xmm1, %xmm11
	movaps	%xmm1, %xmm8
	movaps	%xmm1, %xmm9
	movaps	%xmm1, %xmm6
	movaps	%xmm1, %xmm7
	movaps	%xmm1, %xmm4
	movaps	%xmm1, %xmm5
	movaps	%xmm1, %xmm2
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm0
	jmp	.L314
.LFE30:
	.size	unroll12x12a_combine, .-unroll12x12a_combine
	.p2align 4,,15
.globl unroll12x6a_combine
	.type	unroll12x6a_combine, @function
unroll12x6a_combine:
.LFB29:
	pushq	%r13
.LCFI110:
	movq	%rsi, %r13
	pushq	%r12
.LCFI111:
	pushq	%rbp
.LCFI112:
	pushq	%rbx
.LCFI113:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI114:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-11(%rbp), %r12d
	call	get_vec_start
	movslq	%r12d,%rdx
	movq	%rax, %rsi
	testq	%rdx, %rdx
	jle	.L331
	movss	.LC2(%rip), %xmm0
	xorl	%ecx, %ecx
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm2
	movaps	%xmm0, %xmm5
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm4
	.p2align 4,,10
	.p2align 3
.L325:
	mulss	(%rax), %xmm0
	addq	$12, %rcx
	mulss	4(%rax), %xmm3
	mulss	8(%rax), %xmm2
	mulss	12(%rax), %xmm5
	mulss	16(%rax), %xmm1
	mulss	20(%rax), %xmm4
	mulss	24(%rax), %xmm0
	mulss	28(%rax), %xmm3
	mulss	32(%rax), %xmm2
	mulss	36(%rax), %xmm5
	mulss	40(%rax), %xmm1
	mulss	44(%rax), %xmm4
	addq	$48, %rax
	cmpq	%rdx, %rcx
	jl	.L325
	subq	$1, %rdx
	movabsq	$-6148914691236517205, %rax
	mulq	%rdx
	shrq	$3, %rdx
	leaq	3(%rdx,%rdx,2), %rdx
	salq	$2, %rdx
.L324:
	movslq	%ebp,%rcx
	cmpq	%rdx, %rcx
	jle	.L326
	leaq	(%rsi,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L327:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rcx
	jg	.L327
.L326:
	mulss	%xmm3, %xmm0
	mulss	%xmm5, %xmm2
	mulss	%xmm4, %xmm1
	mulss	%xmm2, %xmm0
	mulss	%xmm1, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L331:
	movss	.LC2(%rip), %xmm3
	xorl	%edx, %edx
	movaps	%xmm3, %xmm2
	movaps	%xmm3, %xmm5
	movaps	%xmm3, %xmm1
	movaps	%xmm3, %xmm4
	movaps	%xmm3, %xmm0
	jmp	.L324
.LFE29:
	.size	unroll12x6a_combine, .-unroll12x6a_combine
	.p2align 4,,15
.globl unroll8x4a_combine
	.type	unroll8x4a_combine, @function
unroll8x4a_combine:
.LFB28:
	pushq	%r13
.LCFI115:
	movq	%rsi, %r13
	pushq	%r12
.LCFI116:
	pushq	%rbp
.LCFI117:
	pushq	%rbx
.LCFI118:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI119:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-7(%rbp), %r12d
	call	get_vec_start
	movq	%rax, %rcx
	movslq	%r12d,%rax
	testq	%rax, %rax
	jle	.L341
	movss	.LC2(%rip), %xmm0
	xorl	%edx, %edx
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm2
	.p2align 4,,10
	.p2align 3
.L335:
	mulss	(%rcx,%rdx,4), %xmm0
	mulss	4(%rcx,%rdx,4), %xmm1
	mulss	8(%rcx,%rdx,4), %xmm3
	mulss	12(%rcx,%rdx,4), %xmm2
	mulss	16(%rcx,%rdx,4), %xmm0
	mulss	20(%rcx,%rdx,4), %xmm1
	mulss	24(%rcx,%rdx,4), %xmm3
	mulss	28(%rcx,%rdx,4), %xmm2
	addq	$8, %rdx
	cmpq	%rax, %rdx
	jl	.L335
	subq	$1, %rax
	shrq	$3, %rax
	leaq	8(,%rax,8), %rdx
.L334:
	movslq	%ebp,%r12
	cmpq	%rdx, %r12
	jle	.L336
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L337:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %r12
	jg	.L337
.L336:
	mulss	%xmm1, %xmm0
	mulss	%xmm3, %xmm0
	mulss	%xmm2, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L341:
	movss	.LC2(%rip), %xmm1
	xorl	%edx, %edx
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm2
	movaps	%xmm1, %xmm0
	jmp	.L334
.LFE28:
	.size	unroll8x4a_combine, .-unroll8x4a_combine
	.p2align 4,,15
.globl unroll4x4a_combine
	.type	unroll4x4a_combine, @function
unroll4x4a_combine:
.LFB27:
	pushq	%r13
.LCFI120:
	movq	%rsi, %r13
	pushq	%r12
.LCFI121:
	pushq	%rbp
.LCFI122:
	pushq	%rbx
.LCFI123:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI124:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-3(%rbp), %r12d
	call	get_vec_start
	movq	%rax, %rcx
	movslq	%r12d,%rax
	testq	%rax, %rax
	jle	.L351
	movss	.LC2(%rip), %xmm0
	xorl	%edx, %edx
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm2
	.p2align 4,,10
	.p2align 3
.L345:
	mulss	(%rcx,%rdx,4), %xmm0
	mulss	4(%rcx,%rdx,4), %xmm1
	mulss	8(%rcx,%rdx,4), %xmm3
	mulss	12(%rcx,%rdx,4), %xmm2
	addq	$4, %rdx
	cmpq	%rax, %rdx
	jl	.L345
	subq	$1, %rax
	shrq	$2, %rax
	leaq	4(,%rax,4), %rdx
.L344:
	movslq	%ebp,%r12
	cmpq	%rdx, %r12
	jle	.L346
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L347:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %r12
	jg	.L347
.L346:
	mulss	%xmm1, %xmm0
	mulss	%xmm3, %xmm0
	mulss	%xmm2, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L351:
	movss	.LC2(%rip), %xmm1
	xorl	%edx, %edx
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm2
	movaps	%xmm1, %xmm0
	jmp	.L344
.LFE27:
	.size	unroll4x4a_combine, .-unroll4x4a_combine
	.p2align 4,,15
.globl unroll3x3a_combine
	.type	unroll3x3a_combine, @function
unroll3x3a_combine:
.LFB26:
	pushq	%r13
.LCFI125:
	movq	%rsi, %r13
	pushq	%r12
.LCFI126:
	pushq	%rbp
.LCFI127:
	pushq	%rbx
.LCFI128:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI129:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-2(%rbp), %r12d
	call	get_vec_start
	movslq	%r12d,%rdx
	movq	%rax, %rcx
	testq	%rdx, %rdx
	jle	.L361
	movss	.LC2(%rip), %xmm0
	xorl	%eax, %eax
	movaps	%xmm0, %xmm2
	movaps	%xmm0, %xmm1
	.p2align 4,,10
	.p2align 3
.L355:
	mulss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm2
	mulss	8(%rcx,%rax,4), %xmm1
	addq	$3, %rax
	cmpq	%rdx, %rax
	jl	.L355
	subq	$1, %rdx
	movabsq	$-6148914691236517205, %rax
	mulq	%rdx
	shrq	%rdx
	leaq	3(%rdx,%rdx,2), %rdx
.L354:
	movslq	%ebp,%r12
	cmpq	%rdx, %r12
	jle	.L356
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L357:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %r12
	jg	.L357
.L356:
	mulss	%xmm2, %xmm0
	mulss	%xmm1, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L361:
	movss	.LC2(%rip), %xmm2
	xorl	%edx, %edx
	movaps	%xmm2, %xmm1
	movaps	%xmm2, %xmm0
	jmp	.L354
.LFE26:
	.size	unroll3x3a_combine, .-unroll3x3a_combine
	.p2align 4,,15
.globl unroll8x2a_combine
	.type	unroll8x2a_combine, @function
unroll8x2a_combine:
.LFB25:
	pushq	%r13
.LCFI130:
	movq	%rsi, %r13
	pushq	%r12
.LCFI131:
	pushq	%rbp
.LCFI132:
	pushq	%rbx
.LCFI133:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI134:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-7(%rbp), %r12d
	call	get_vec_start
	movq	%rax, %rcx
	movslq	%r12d,%rax
	testq	%rax, %rax
	jle	.L371
	movss	.LC2(%rip), %xmm0
	xorl	%edx, %edx
	movaps	%xmm0, %xmm1
	.p2align 4,,10
	.p2align 3
.L365:
	mulss	(%rcx,%rdx,4), %xmm0
	mulss	4(%rcx,%rdx,4), %xmm1
	mulss	8(%rcx,%rdx,4), %xmm0
	mulss	12(%rcx,%rdx,4), %xmm1
	mulss	16(%rcx,%rdx,4), %xmm0
	mulss	20(%rcx,%rdx,4), %xmm1
	mulss	24(%rcx,%rdx,4), %xmm0
	mulss	28(%rcx,%rdx,4), %xmm1
	addq	$8, %rdx
	cmpq	%rax, %rdx
	jl	.L365
	subq	$1, %rax
	shrq	$3, %rax
	leaq	8(,%rax,8), %rdx
.L364:
	movslq	%ebp,%r12
	cmpq	%rdx, %r12
	jle	.L366
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L367:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %r12
	jg	.L367
.L366:
	mulss	%xmm1, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L371:
	movss	.LC2(%rip), %xmm1
	xorl	%edx, %edx
	movaps	%xmm1, %xmm0
	jmp	.L364
.LFE25:
	.size	unroll8x2a_combine, .-unroll8x2a_combine
	.p2align 4,,15
.globl unroll4x2a_combine
	.type	unroll4x2a_combine, @function
unroll4x2a_combine:
.LFB24:
	pushq	%r13
.LCFI135:
	movq	%rsi, %r13
	pushq	%r12
.LCFI136:
	pushq	%rbp
.LCFI137:
	pushq	%rbx
.LCFI138:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI139:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-3(%rbp), %r12d
	call	get_vec_start
	movq	%rax, %rcx
	movslq	%r12d,%rax
	testq	%rax, %rax
	jle	.L381
	movss	.LC2(%rip), %xmm0
	xorl	%edx, %edx
	movaps	%xmm0, %xmm1
	.p2align 4,,10
	.p2align 3
.L375:
	mulss	(%rcx,%rdx,4), %xmm0
	mulss	4(%rcx,%rdx,4), %xmm1
	mulss	8(%rcx,%rdx,4), %xmm0
	mulss	12(%rcx,%rdx,4), %xmm1
	addq	$4, %rdx
	cmpq	%rax, %rdx
	jl	.L375
	subq	$1, %rax
	shrq	$2, %rax
	leaq	4(,%rax,4), %rdx
.L374:
	movslq	%ebp,%r12
	cmpq	%rdx, %r12
	jle	.L376
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L377:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %r12
	jg	.L377
.L376:
	mulss	%xmm1, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L381:
	movss	.LC2(%rip), %xmm1
	xorl	%edx, %edx
	movaps	%xmm1, %xmm0
	jmp	.L374
.LFE24:
	.size	unroll4x2a_combine, .-unroll4x2a_combine
	.p2align 4,,15
.globl combine6
	.type	combine6, @function
combine6:
.LFB23:
	pushq	%r13
.LCFI140:
	movq	%rsi, %r13
	pushq	%r12
.LCFI141:
	pushq	%rbp
.LCFI142:
	pushq	%rbx
.LCFI143:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI144:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-1(%rbp), %r12d
	call	get_vec_start
	movq	%rax, %rcx
	movslq	%r12d,%rax
	testq	%rax, %rax
	jle	.L391
	movss	.LC2(%rip), %xmm0
	xorl	%edx, %edx
	movaps	%xmm0, %xmm1
	.p2align 4,,10
	.p2align 3
.L385:
	mulss	(%rcx,%rdx,4), %xmm0
	mulss	4(%rcx,%rdx,4), %xmm1
	addq	$2, %rdx
	cmpq	%rax, %rdx
	jl	.L385
	subq	$1, %rax
	shrq	%rax
	leaq	2(%rax,%rax), %rdx
.L384:
	movslq	%ebp,%r12
	cmpq	%rdx, %r12
	jle	.L386
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L387:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%r12, %rdx
	jl	.L387
.L386:
	mulss	%xmm1, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L391:
	movss	.LC2(%rip), %xmm1
	xorl	%edx, %edx
	movaps	%xmm1, %xmm0
	jmp	.L384
.LFE23:
	.size	combine6, .-combine6
	.p2align 4,,15
.globl unroll16_combine
	.type	unroll16_combine, @function
unroll16_combine:
.LFB22:
	pushq	%r12
.LCFI145:
	movq	%rdi, %r12
	pushq	%rbp
.LCFI146:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI147:
	call	vec_length
	movq	%r12, %rdi
	movl	%eax, %ebx
	call	get_vec_start
	movl	%ebx, %edx
	movq	%rax, %rcx
	sarl	$31, %edx
	shrl	$28, %edx
	leal	(%rbx,%rdx), %eax
	movslq	%ebx,%rbx
	andl	$15, %eax
	subl	%edx, %eax
	movslq	%eax,%rsi
	subq	%rsi, %rbx
	leaq	(%rcx,%rbx,4), %rdx
	cmpq	%rdx, %rcx
	jae	.L402
	movq	%rcx, %rax
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L395:
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm0
	mulss	8(%rax), %xmm0
	mulss	12(%rax), %xmm0
	mulss	16(%rax), %xmm0
	mulss	20(%rax), %xmm0
	mulss	24(%rax), %xmm0
	mulss	28(%rax), %xmm0
	mulss	32(%rax), %xmm0
	mulss	36(%rax), %xmm0
	mulss	40(%rax), %xmm0
	mulss	44(%rax), %xmm0
	mulss	48(%rax), %xmm0
	mulss	52(%rax), %xmm0
	mulss	56(%rax), %xmm0
	mulss	60(%rax), %xmm0
	addq	$64, %rax
	cmpq	%rax, %rdx
	ja	.L395
	movq	%rcx, %rax
	notq	%rax
	addq	%rdx, %rax
	andq	$-64, %rax
	leaq	64(%rcx,%rax), %rcx
.L394:
	leaq	(%rdx,%rsi,4), %rax
	cmpq	%rcx, %rax
	jbe	.L396
	.p2align 4,,10
	.p2align 3
.L399:
	mulss	(%rcx), %xmm0
	addq	$4, %rcx
	cmpq	%rcx, %rax
	ja	.L399
.L396:
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L402:
	movss	.LC2(%rip), %xmm0
	jmp	.L394
.LFE22:
	.size	unroll16_combine, .-unroll16_combine
	.p2align 4,,15
.globl unroll8_combine
	.type	unroll8_combine, @function
unroll8_combine:
.LFB21:
	pushq	%r12
.LCFI148:
	movq	%rdi, %r12
	pushq	%rbp
.LCFI149:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI150:
	call	vec_length
	movq	%r12, %rdi
	movl	%eax, %ebx
	call	get_vec_start
	movl	%ebx, %edx
	movq	%rax, %rcx
	sarl	$31, %edx
	shrl	$29, %edx
	leal	(%rbx,%rdx), %eax
	movslq	%ebx,%rbx
	andl	$7, %eax
	subl	%edx, %eax
	movslq	%eax,%rsi
	subq	%rsi, %rbx
	leaq	(%rcx,%rbx,4), %rdx
	cmpq	%rdx, %rcx
	jae	.L413
	movq	%rcx, %rax
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L406:
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm0
	mulss	8(%rax), %xmm0
	mulss	12(%rax), %xmm0
	mulss	16(%rax), %xmm0
	mulss	20(%rax), %xmm0
	mulss	24(%rax), %xmm0
	mulss	28(%rax), %xmm0
	addq	$32, %rax
	cmpq	%rax, %rdx
	ja	.L406
	movq	%rcx, %rax
	notq	%rax
	addq	%rdx, %rax
	andq	$-32, %rax
	leaq	32(%rcx,%rax), %rcx
.L405:
	leaq	(%rdx,%rsi,4), %rax
	cmpq	%rcx, %rax
	jbe	.L407
	.p2align 4,,10
	.p2align 3
.L410:
	mulss	(%rcx), %xmm0
	addq	$4, %rcx
	cmpq	%rcx, %rax
	ja	.L410
.L407:
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L413:
	movss	.LC2(%rip), %xmm0
	jmp	.L405
.LFE21:
	.size	unroll8_combine, .-unroll8_combine
	.p2align 4,,15
.globl unroll4_combine
	.type	unroll4_combine, @function
unroll4_combine:
.LFB20:
	pushq	%r12
.LCFI151:
	movq	%rdi, %r12
	pushq	%rbp
.LCFI152:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI153:
	call	vec_length
	movl	%eax, %ebx
	movq	%r12, %rdi
	movslq	%ebx,%rbx
	call	get_vec_start
	leaq	-12(%rax,%rbx,4), %rcx
	movq	%rax, %rdx
	cmpq	%rcx, %rax
	jae	.L424
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L417:
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm0
	mulss	8(%rax), %xmm0
	mulss	12(%rax), %xmm0
	addq	$16, %rax
	cmpq	%rax, %rcx
	ja	.L417
	movq	%rdx, %rax
	notq	%rax
	addq	%rcx, %rax
	andq	$-16, %rax
	leaq	16(%rdx,%rax), %rdx
.L416:
	leaq	12(%rcx), %rax
	cmpq	%rdx, %rax
	jbe	.L418
	.p2align 4,,10
	.p2align 3
.L421:
	mulss	(%rdx), %xmm0
	addq	$4, %rdx
	cmpq	%rdx, %rax
	ja	.L421
.L418:
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L424:
	movss	.LC2(%rip), %xmm0
	jmp	.L416
.LFE20:
	.size	unroll4_combine, .-unroll4_combine
	.p2align 4,,15
.globl unroll3_combine
	.type	unroll3_combine, @function
unroll3_combine:
.LFB19:
	pushq	%r12
.LCFI154:
	movq	%rdi, %r12
	pushq	%rbp
.LCFI155:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI156:
	call	vec_length
	movl	%eax, %ebx
	movq	%r12, %rdi
	movslq	%ebx,%rbx
	call	get_vec_start
	leaq	-8(%rax,%rbx,4), %rsi
	movq	%rax, %rcx
	cmpq	%rsi, %rax
	jae	.L435
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L428:
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm0
	mulss	8(%rax), %xmm0
	addq	$12, %rax
	cmpq	%rax, %rsi
	ja	.L428
	movq	%rcx, %rdx
	movabsq	$-6148914691236517205, %rax
	notq	%rdx
	addq	%rsi, %rdx
	mulq	%rdx
	shrq	$3, %rdx
	leaq	3(%rdx,%rdx,2), %rdx
	leaq	(%rcx,%rdx,4), %rcx
.L427:
	leaq	8(%rsi), %rax
	cmpq	%rcx, %rax
	jbe	.L429
	.p2align 4,,10
	.p2align 3
.L432:
	mulss	(%rcx), %xmm0
	addq	$4, %rcx
	cmpq	%rcx, %rax
	ja	.L432
.L429:
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L435:
	movss	.LC2(%rip), %xmm0
	jmp	.L427
.LFE19:
	.size	unroll3_combine, .-unroll3_combine
	.p2align 4,,15
.globl unroll2_combine
	.type	unroll2_combine, @function
unroll2_combine:
.LFB18:
	pushq	%r12
.LCFI157:
	movq	%rdi, %r12
	pushq	%rbp
.LCFI158:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI159:
	call	vec_length
	movq	%r12, %rdi
	movl	%eax, %ebx
	call	get_vec_start
	movl	%ebx, %edx
	movq	%rax, %rcx
	shrl	$31, %edx
	leal	(%rbx,%rdx), %eax
	movslq	%ebx,%rbx
	andl	$1, %eax
	subl	%edx, %eax
	movslq	%eax,%rsi
	subq	%rsi, %rbx
	leaq	(%rcx,%rbx,4), %rdx
	cmpq	%rdx, %rcx
	jae	.L446
	movq	%rcx, %rax
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L439:
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm0
	addq	$8, %rax
	cmpq	%rax, %rdx
	ja	.L439
	movq	%rcx, %rax
	notq	%rax
	addq	%rdx, %rax
	shrq	$3, %rax
	leaq	8(%rcx,%rax,8), %rcx
.L438:
	leaq	(%rdx,%rsi,4), %rax
	cmpq	%rcx, %rax
	jbe	.L440
	.p2align 4,,10
	.p2align 3
.L443:
	mulss	(%rcx), %xmm0
	addq	$4, %rcx
	cmpq	%rcx, %rax
	ja	.L443
.L440:
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L446:
	movss	.LC2(%rip), %xmm0
	jmp	.L438
.LFE18:
	.size	unroll2_combine, .-unroll2_combine
	.p2align 4,,15
.globl unroll16a_combine
	.type	unroll16a_combine, @function
unroll16a_combine:
.LFB17:
	pushq	%r13
.LCFI160:
	movq	%rsi, %r13
	pushq	%r12
.LCFI161:
	pushq	%rbp
.LCFI162:
	pushq	%rbx
.LCFI163:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI164:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-15(%rbp), %r12d
	call	get_vec_start
	movq	%rax, %rsi
	movslq	%r12d,%rax
	testq	%rax, %rax
	jle	.L456
	movq	%rsi, %rdx
	xorl	%ecx, %ecx
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L450:
	mulss	(%rdx), %xmm0
	addq	$16, %rcx
	mulss	4(%rdx), %xmm0
	mulss	8(%rdx), %xmm0
	mulss	12(%rdx), %xmm0
	mulss	16(%rdx), %xmm0
	mulss	20(%rdx), %xmm0
	mulss	24(%rdx), %xmm0
	mulss	28(%rdx), %xmm0
	mulss	32(%rdx), %xmm0
	mulss	36(%rdx), %xmm0
	mulss	40(%rdx), %xmm0
	mulss	44(%rdx), %xmm0
	mulss	48(%rdx), %xmm0
	mulss	52(%rdx), %xmm0
	mulss	56(%rdx), %xmm0
	mulss	60(%rdx), %xmm0
	addq	$64, %rdx
	cmpq	%rcx, %rax
	jg	.L450
	subq	$1, %rax
	andq	$-16, %rax
	leaq	16(%rax), %rdx
.L449:
	movslq	%ebp,%rcx
	cmpq	%rcx, %rdx
	jge	.L451
	leaq	(%rsi,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L452:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rcx
	jg	.L452
.L451:
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L456:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	jmp	.L449
.LFE17:
	.size	unroll16a_combine, .-unroll16a_combine
	.p2align 4,,15
.globl unroll8a_combine
	.type	unroll8a_combine, @function
unroll8a_combine:
.LFB16:
	pushq	%r13
.LCFI165:
	movq	%rsi, %r13
	pushq	%r12
.LCFI166:
	pushq	%rbp
.LCFI167:
	pushq	%rbx
.LCFI168:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI169:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-7(%rbp), %r12d
	call	get_vec_start
	movq	%rax, %rcx
	movslq	%r12d,%rax
	testq	%rax, %rax
	jle	.L466
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L460:
	mulss	(%rcx,%rdx,4), %xmm0
	mulss	4(%rcx,%rdx,4), %xmm0
	mulss	8(%rcx,%rdx,4), %xmm0
	mulss	12(%rcx,%rdx,4), %xmm0
	mulss	16(%rcx,%rdx,4), %xmm0
	mulss	20(%rcx,%rdx,4), %xmm0
	mulss	24(%rcx,%rdx,4), %xmm0
	mulss	28(%rcx,%rdx,4), %xmm0
	addq	$8, %rdx
	cmpq	%rax, %rdx
	jl	.L460
	subq	$1, %rax
	shrq	$3, %rax
	leaq	8(,%rax,8), %rdx
.L459:
	movslq	%ebp,%r12
	cmpq	%r12, %rdx
	jge	.L461
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L462:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %r12
	jg	.L462
.L461:
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L466:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	jmp	.L459
.LFE16:
	.size	unroll8a_combine, .-unroll8a_combine
	.p2align 4,,15
.globl unroll6a_combine
	.type	unroll6a_combine, @function
unroll6a_combine:
.LFB15:
	pushq	%r13
.LCFI170:
	movq	%rsi, %r13
	pushq	%r12
.LCFI171:
	pushq	%rbp
.LCFI172:
	pushq	%rbx
.LCFI173:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI174:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-5(%rbp), %r12d
	call	get_vec_start
	movslq	%r12d,%rdx
	movq	%rax, %rcx
	testq	%rdx, %rdx
	jle	.L476
	xorl	%eax, %eax
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L470:
	mulss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm0
	mulss	8(%rcx,%rax,4), %xmm0
	mulss	12(%rcx,%rax,4), %xmm0
	mulss	16(%rcx,%rax,4), %xmm0
	mulss	20(%rcx,%rax,4), %xmm0
	addq	$6, %rax
	cmpq	%rdx, %rax
	jl	.L470
	subq	$1, %rdx
	movabsq	$-6148914691236517205, %rax
	mulq	%rdx
	shrq	$2, %rdx
	leaq	3(%rdx,%rdx,2), %rdx
	addq	%rdx, %rdx
.L469:
	movslq	%ebp,%r12
	cmpq	%r12, %rdx
	jge	.L471
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L472:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %r12
	jg	.L472
.L471:
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L476:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	jmp	.L469
.LFE15:
	.size	unroll6a_combine, .-unroll6a_combine
	.p2align 4,,15
.globl unroll5a_combine
	.type	unroll5a_combine, @function
unroll5a_combine:
.LFB14:
	pushq	%r13
.LCFI175:
	movq	%rsi, %r13
	pushq	%r12
.LCFI176:
	pushq	%rbp
.LCFI177:
	pushq	%rbx
.LCFI178:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI179:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-4(%rbp), %r12d
	call	get_vec_start
	movslq	%r12d,%rdx
	movq	%rax, %rcx
	testq	%rdx, %rdx
	jle	.L486
	xorl	%eax, %eax
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L480:
	mulss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm0
	mulss	8(%rcx,%rax,4), %xmm0
	mulss	12(%rcx,%rax,4), %xmm0
	mulss	16(%rcx,%rax,4), %xmm0
	addq	$5, %rax
	cmpq	%rdx, %rax
	jl	.L480
	subq	$1, %rdx
	movabsq	$-3689348814741910323, %rax
	mulq	%rdx
	shrq	$2, %rdx
	leaq	5(%rdx,%rdx,4), %rdx
.L479:
	movslq	%ebp,%r12
	cmpq	%r12, %rdx
	jge	.L481
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L482:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %r12
	jg	.L482
.L481:
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L486:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	jmp	.L479
.LFE14:
	.size	unroll5a_combine, .-unroll5a_combine
	.p2align 4,,15
.globl unroll4a_combine
	.type	unroll4a_combine, @function
unroll4a_combine:
.LFB13:
	pushq	%r13
.LCFI180:
	movq	%rsi, %r13
	pushq	%r12
.LCFI181:
	pushq	%rbp
.LCFI182:
	pushq	%rbx
.LCFI183:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI184:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-3(%rbp), %r12d
	call	get_vec_start
	movq	%rax, %rcx
	movslq	%r12d,%rax
	testq	%rax, %rax
	jle	.L496
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L490:
	mulss	(%rcx,%rdx,4), %xmm0
	mulss	4(%rcx,%rdx,4), %xmm0
	mulss	8(%rcx,%rdx,4), %xmm0
	mulss	12(%rcx,%rdx,4), %xmm0
	addq	$4, %rdx
	cmpq	%rax, %rdx
	jl	.L490
	subq	$1, %rax
	shrq	$2, %rax
	leaq	4(,%rax,4), %rdx
.L489:
	movslq	%ebp,%r12
	cmpq	%r12, %rdx
	jge	.L491
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L492:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%r12, %rdx
	jl	.L492
.L491:
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L496:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	jmp	.L489
.LFE13:
	.size	unroll4a_combine, .-unroll4a_combine
	.p2align 4,,15
.globl unroll2aw_combine
	.type	unroll2aw_combine, @function
unroll2aw_combine:
.LFB12:
	pushq	%r13
.LCFI185:
	movq	%rsi, %r13
	pushq	%r12
.LCFI186:
	pushq	%rbp
.LCFI187:
	pushq	%rbx
.LCFI188:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI189:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-1(%rbp), %r12d
	call	get_vec_start
	movq	%rax, %rcx
	movslq	%r12d,%rax
	testq	%rax, %rax
	jle	.L506
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L500:
	mulss	(%rcx,%rdx,4), %xmm0
	addq	$2, %rdx
	cmpq	%rax, %rdx
	mulss	-4(%rcx,%rdx,4), %xmm0
	jl	.L500
	subq	$1, %rax
	shrq	%rax
	leaq	2(%rax,%rax), %rdx
.L499:
	movslq	%ebp,%r12
	cmpq	%r12, %rdx
	jge	.L501
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L502:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%r12, %rdx
	jl	.L502
.L501:
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L506:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	jmp	.L499
.LFE12:
	.size	unroll2aw_combine, .-unroll2aw_combine
	.p2align 4,,15
.globl combine5p
	.type	combine5p, @function
combine5p:
.LFB11:
	pushq	%r12
.LCFI190:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI191:
	pushq	%rbx
.LCFI192:
	movq	%rdi, %rbx
	call	get_vec_start
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	vec_length
	cltq
	leaq	(%rbp,%rax,4), %rdx
	leaq	-4(%rdx), %rcx
	cmpq	%rcx, %rbp
	jae	.L517
	movq	%rbp, %rax
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L510:
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm0
	addq	$8, %rax
	cmpq	%rax, %rcx
	ja	.L510
	movq	%rdx, %rax
	subq	%rbp, %rax
	subq	$5, %rax
	shrq	$3, %rax
	leaq	8(%rbp,%rax,8), %rbp
.L509:
	cmpq	%rdx, %rbp
	jae	.L511
	.p2align 4,,10
	.p2align 3
.L514:
	mulss	(%rbp), %xmm0
	addq	$4, %rbp
	cmpq	%rbp, %rdx
	ja	.L514
.L511:
	popq	%rbx
	popq	%rbp
	movss	%xmm0, (%r12)
	popq	%r12
	ret
.L517:
	movss	.LC2(%rip), %xmm0
	jmp	.L509
.LFE11:
	.size	combine5p, .-combine5p
	.p2align 4,,15
.globl unroll3a_combine
	.type	unroll3a_combine, @function
unroll3a_combine:
.LFB10:
	pushq	%r13
.LCFI193:
	movq	%rsi, %r13
	pushq	%r12
.LCFI194:
	pushq	%rbp
.LCFI195:
	pushq	%rbx
.LCFI196:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI197:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-2(%rbp), %r12d
	call	get_vec_start
	movslq	%r12d,%rdx
	movq	%rax, %rcx
	testq	%rdx, %rdx
	jle	.L527
	xorl	%eax, %eax
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L521:
	mulss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm0
	mulss	8(%rcx,%rax,4), %xmm0
	addq	$3, %rax
	cmpq	%rdx, %rax
	jl	.L521
	subq	$1, %rdx
	movabsq	$-6148914691236517205, %rax
	mulq	%rdx
	shrq	%rdx
	leaq	3(%rdx,%rdx,2), %rdx
.L520:
	movslq	%ebp,%r12
	cmpq	%r12, %rdx
	jge	.L522
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L523:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%r12, %rdx
	jl	.L523
.L522:
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L527:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	jmp	.L520
.LFE10:
	.size	unroll3a_combine, .-unroll3a_combine
	.p2align 4,,15
.globl combine5
	.type	combine5, @function
combine5:
.LFB9:
	pushq	%r13
.LCFI198:
	movq	%rsi, %r13
	pushq	%r12
.LCFI199:
	pushq	%rbp
.LCFI200:
	pushq	%rbx
.LCFI201:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI202:
	call	vec_length
	movl	%eax, %ebp
	movq	%rbx, %rdi
	leal	-1(%rbp), %r12d
	call	get_vec_start
	movq	%rax, %rcx
	movslq	%r12d,%rax
	testq	%rax, %rax
	jle	.L537
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L531:
	mulss	(%rcx,%rdx,4), %xmm0
	mulss	4(%rcx,%rdx,4), %xmm0
	addq	$2, %rdx
	cmpq	%rax, %rdx
	jl	.L531
	subq	$1, %rax
	shrq	%rax
	leaq	2(%rax,%rax), %rdx
.L530:
	movslq	%ebp,%r12
	cmpq	%r12, %rdx
	jge	.L532
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L533:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%r12, %rdx
	jl	.L533
.L532:
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L537:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	jmp	.L530
.LFE9:
	.size	combine5, .-combine5
	.p2align 4,,15
.globl combine4p
	.type	combine4p, @function
combine4p:
.LFB8:
	pushq	%r12
.LCFI203:
	movq	%rdi, %r12
	pushq	%rbp
.LCFI204:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI205:
	call	vec_length
	movl	%eax, %ebx
	movq	%r12, %rdi
	movslq	%ebx,%rbx
	call	get_vec_start
	leaq	(%rax,%rbx,4), %rdx
	cmpq	%rdx, %rax
	jae	.L544
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L541:
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rax, %rdx
	ja	.L541
.L540:
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L544:
	movss	.LC2(%rip), %xmm0
	jmp	.L540
.LFE8:
	.size	combine4p, .-combine4p
	.p2align 4,,15
.globl combine4b
	.type	combine4b, @function
combine4b:
.LFB7:
	pushq	%rbp
.LCFI206:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI207:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI208:
	call	vec_length
	movslq	%eax,%rcx
	testq	%rcx, %rcx
	jle	.L552
	movslq	(%rbx),%rsi
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L549:
	cmpq	%rdx, %rsi
	jle	.L548
	movq	8(%rbx), %rax
	mulss	(%rax,%rdx,4), %xmm0
.L548:
	addq	$1, %rdx
	cmpq	%rcx, %rdx
	jl	.L549
.L547:
	movss	%xmm0, (%rbp)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	ret
.L552:
	movss	.LC2(%rip), %xmm0
	jmp	.L547
.LFE7:
	.size	combine4b, .-combine4b
	.p2align 4,,15
.globl combine4
	.type	combine4, @function
combine4:
.LFB6:
	pushq	%r12
.LCFI209:
	pushq	%rbp
.LCFI210:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI211:
	movq	%rdi, %rbx
	call	vec_length
	movq	%rbx, %rdi
	movl	%eax, %r12d
	call	get_vec_start
	movslq	%r12d,%rcx
	testq	%rcx, %rcx
	jle	.L559
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L556:
	mulss	(%rax,%rdx,4), %xmm0
	addq	$1, %rdx
	cmpq	%rcx, %rdx
	jl	.L556
.L555:
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L559:
	movss	.LC2(%rip), %xmm0
	jmp	.L555
.LFE6:
	.size	combine4, .-combine4
	.p2align 4,,15
.globl combine3w
	.type	combine3w, @function
combine3w:
.LFB5:
	pushq	%r12
.LCFI212:
	pushq	%rbp
.LCFI213:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI214:
	movq	%rdi, %rbx
	call	vec_length
	movq	%rbx, %rdi
	movl	%eax, %r12d
	call	get_vec_start
	movslq	%r12d,%rcx
	testq	%rcx, %rcx
	jle	.L563
	movss	.LC2(%rip), %xmm0
	xorl	%edx, %edx
	.p2align 4,,10
	.p2align 3
.L562:
	mulss	(%rax,%rdx,4), %xmm0
	addq	$1, %rdx
	cmpq	%rcx, %rdx
	movss	%xmm0, (%rbp)
	jl	.L562
.L563:
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.LFE5:
	.size	combine3w, .-combine3w
	.p2align 4,,15
.globl combine3
	.type	combine3, @function
combine3:
.LFB4:
	pushq	%r12
.LCFI215:
	pushq	%rbp
.LCFI216:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI217:
	movq	%rdi, %rbx
	call	vec_length
	movq	%rbx, %rdi
	movl	%eax, %r12d
	call	get_vec_start
	movslq	%r12d,%rcx
	movss	.LC2(%rip), %xmm0
	testq	%rcx, %rcx
	movss	%xmm0, (%rbp)
	jle	.L568
	xorl	%edx, %edx
	.p2align 4,,10
	.p2align 3
.L567:
	mulss	(%rax,%rdx,4), %xmm0
	addq	$1, %rdx
	cmpq	%rcx, %rdx
	movss	%xmm0, (%rbp)
	jl	.L567
.L568:
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.LFE4:
	.size	combine3, .-combine3
	.p2align 4,,15
.globl combine2
	.type	combine2, @function
combine2:
.LFB3:
	pushq	%r14
.LCFI218:
	pushq	%r13
.LCFI219:
	movq	%rdi, %r13
	pushq	%r12
.LCFI220:
	pushq	%rbp
.LCFI221:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI222:
	subq	$16, %rsp
.LCFI223:
	call	vec_length
	movslq	%eax,%r12
	movl	$0x3f800000, (%rbp)
	testq	%r12, %r12
	jle	.L573
	leaq	12(%rsp), %r14
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L572:
	movl	%ebx, %esi
	movq	%r14, %rdx
	movq	%r13, %rdi
	call	get_vec_element
	movss	(%rbp), %xmm0
	addq	$1, %rbx
	mulss	12(%rsp), %xmm0
	cmpq	%r12, %rbx
	movss	%xmm0, (%rbp)
	jl	.L572
.L573:
	addq	$16, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	ret
.LFE3:
	.size	combine2, .-combine2
	.p2align 4,,15
.globl combine1
	.type	combine1, @function
combine1:
.LFB2:
	pushq	%r13
.LCFI224:
	pushq	%r12
.LCFI225:
	movq	%rdi, %r12
	pushq	%rbp
.LCFI226:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI227:
	xorl	%ebx, %ebx
	subq	$24, %rsp
.LCFI228:
	movl	$0x3f800000, (%rsi)
	leaq	20(%rsp), %r13
	jmp	.L576
	.p2align 4,,10
	.p2align 3
.L577:
	movl	%ebx, %esi
	movq	%r13, %rdx
	movq	%r12, %rdi
	call	get_vec_element
	movss	(%rbp), %xmm0
	addq	$1, %rbx
	mulss	20(%rsp), %xmm0
	movss	%xmm0, (%rbp)
.L576:
	movq	%r12, %rdi
	call	vec_length
	cltq
	cmpq	%rax, %rbx
	jl	.L577
	addq	$24, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.LFE2:
	.size	combine1, .-combine1
.globl combine1_descr
	.data
	.align 32
	.type	combine1_descr, @object
	.size	combine1_descr, 42
combine1_descr:
	.string	"combine1: Maximum use of data abstraction"
.globl combine2_descr
	.align 32
	.type	combine2_descr, @object
	.size	combine2_descr, 40
combine2_descr:
	.string	"combine2: Take vec_length() out of loop"
.globl combine3_descr
	.align 32
	.type	combine3_descr, @object
	.size	combine3_descr, 41
combine3_descr:
	.string	"combine3: Array reference to vector data"
.globl combine3w_descr
	.align 32
	.type	combine3w_descr, @object
	.size	combine3w_descr, 52
combine3w_descr:
	.string	"combine3w: Update *dest within loop only with write"
.globl combine4_descr
	.align 32
	.type	combine4_descr, @object
	.size	combine4_descr, 51
combine4_descr:
	.string	"combine4: Array reference, accumulate in temporary"
.globl combine4b_descr
	.align 32
	.type	combine4b_descr, @object
	.size	combine4b_descr, 39
combine4b_descr:
	.string	"combine4b: Include bonds check in loop"
.globl combine4p_descr
	.align 32
	.type	combine4p_descr, @object
	.size	combine4p_descr, 54
combine4p_descr:
	.string	"combine4p: Pointer reference, accumulate in temporary"
.globl combine5_descr
	.align 32
	.type	combine5_descr, @object
	.size	combine5_descr, 36
combine5_descr:
	.string	"combine5: Array code, unrolled by 2"
.globl unroll3a_descr
	.align 32
	.type	unroll3a_descr, @object
	.size	unroll3a_descr, 36
unroll3a_descr:
	.string	"unroll3a: Array code, unrolled by 3"
.globl combine5p_descr
	.align 32
	.type	combine5p_descr, @object
	.size	combine5p_descr, 49
combine5p_descr:
	.string	"combine5p: Pointer code, unrolled by 2, for loop"
.globl unroll2aw_descr
	.align 32
	.type	unroll2aw_descr, @object
	.size	unroll2aw_descr, 49
unroll2aw_descr:
	.string	"unroll2aw: Array code, unrolled by 2, while loop"
.globl unroll4a_descr
	.align 32
	.type	unroll4a_descr, @object
	.size	unroll4a_descr, 36
unroll4a_descr:
	.string	"unroll4a: Array code, unrolled by 4"
.globl unroll5a_descr
	.align 32
	.type	unroll5a_descr, @object
	.size	unroll5a_descr, 36
unroll5a_descr:
	.string	"unroll5a: Array code, unrolled by 5"
.globl unroll6a_descr
	.align 32
	.type	unroll6a_descr, @object
	.size	unroll6a_descr, 36
unroll6a_descr:
	.string	"unroll6a: Array code, unrolled by 6"
.globl unroll8a_descr
	.align 32
	.type	unroll8a_descr, @object
	.size	unroll8a_descr, 36
unroll8a_descr:
	.string	"unroll8a: Array code, unrolled by 8"
.globl unroll16a_descr
	.align 32
	.type	unroll16a_descr, @object
	.size	unroll16a_descr, 38
unroll16a_descr:
	.string	"unroll16a: Array code, unrolled by 16"
.globl unroll2_descr
	.align 32
	.type	unroll2_descr, @object
	.size	unroll2_descr, 37
unroll2_descr:
	.string	"unroll2: Pointer code, unrolled by 2"
.globl unroll3_descr
	.align 32
	.type	unroll3_descr, @object
	.size	unroll3_descr, 37
unroll3_descr:
	.string	"unroll3: Pointer code, unrolled by 3"
.globl unroll4_descr
	.align 32
	.type	unroll4_descr, @object
	.size	unroll4_descr, 37
unroll4_descr:
	.string	"unroll4: Pointer code, unrolled by 4"
.globl unroll8_descr
	.align 32
	.type	unroll8_descr, @object
	.size	unroll8_descr, 37
unroll8_descr:
	.string	"unroll8: Pointer code, unrolled by 8"
.globl unroll16_descr
	.align 32
	.type	unroll16_descr, @object
	.size	unroll16_descr, 39
unroll16_descr:
	.string	"unroll16: Pointer code, unrolled by 16"
.globl combine6_descr
	.align 32
	.type	combine6_descr, @object
	.size	combine6_descr, 52
combine6_descr:
	.string	"combine6: Array code, unrolled by 2, Superscalar x2"
.globl unroll4x2a_descr
	.align 32
	.type	unroll4x2a_descr, @object
	.size	unroll4x2a_descr, 54
unroll4x2a_descr:
	.string	"unroll4x2a: Array code, unrolled by 4, Superscalar x2"
.globl unroll8x2a_descr
	.align 32
	.type	unroll8x2a_descr, @object
	.size	unroll8x2a_descr, 54
unroll8x2a_descr:
	.string	"unroll8x2a: Array code, unrolled by 8, Superscalar x2"
.globl unroll3x3a_descr
	.align 32
	.type	unroll3x3a_descr, @object
	.size	unroll3x3a_descr, 54
unroll3x3a_descr:
	.string	"unroll3x3a: Array code, unrolled by 3, Superscalar x3"
.globl unroll4x4a_descr
	.align 32
	.type	unroll4x4a_descr, @object
	.size	unroll4x4a_descr, 54
unroll4x4a_descr:
	.string	"unroll4x4a: Array code, unrolled by 4, Superscalar x4"
.globl unroll8x4a_descr
	.align 32
	.type	unroll8x4a_descr, @object
	.size	unroll8x4a_descr, 54
unroll8x4a_descr:
	.string	"unroll8x4a: Array code, unrolled by 8, Superscalar x4"
.globl unroll12x6a_descr
	.align 32
	.type	unroll12x6a_descr, @object
	.size	unroll12x6a_descr, 55
unroll12x6a_descr:
	.string	"unroll2x6a: Array code, unrolled by 12, Superscalar x6"
.globl unroll12x12a_descr
	.align 32
	.type	unroll12x12a_descr, @object
	.size	unroll12x12a_descr, 58
unroll12x12a_descr:
	.string	"unroll12x12a: Array code, unrolled by 12, Superscalar x12"
.globl unroll5x5a_descr
	.align 32
	.type	unroll5x5a_descr, @object
	.size	unroll5x5a_descr, 54
unroll5x5a_descr:
	.string	"unroll5x5a: Array code, unrolled by 5, Superscalar x5"
.globl unroll6x6a_descr
	.align 32
	.type	unroll6x6a_descr, @object
	.size	unroll6x6a_descr, 54
unroll6x6a_descr:
	.string	"unroll6x6a: Array code, unrolled by 6, Superscalar x6"
.globl unroll8x8a_descr
	.align 32
	.type	unroll8x8a_descr, @object
	.size	unroll8x8a_descr, 54
unroll8x8a_descr:
	.string	"unroll8x8a: Array code, unrolled by 8, Superscalar x8"
.globl unroll10x10a_descr
	.align 32
	.type	unroll10x10a_descr, @object
	.size	unroll10x10a_descr, 58
unroll10x10a_descr:
	.string	"unroll10x10a: Array code, unrolled by 10, Superscalar x10"
.globl unrollx2as_descr
	.align 32
	.type	unrollx2as_descr, @object
	.size	unrollx2as_descr, 66
unrollx2as_descr:
	.string	"unrollx2as: Array code, Unroll x2, Superscalar x2, noninterleaved"
.globl unroll4x2as_descr
	.align 32
	.type	unroll4x2as_descr, @object
	.size	unroll4x2as_descr, 67
unroll4x2as_descr:
	.string	"unroll4x2as: Array code, Unroll x4, Superscalar x2, noninterleaved"
.globl unroll8x2_descr
	.align 32
	.type	unroll8x2_descr, @object
	.size	unroll8x2_descr, 55
unroll8x2_descr:
	.string	"unroll8x2: Pointer code, unrolled by 8, Superscalar x2"
.globl unroll9x3_descr
	.align 32
	.type	unroll9x3_descr, @object
	.size	unroll9x3_descr, 55
unroll9x3_descr:
	.string	"unroll9x3: Pointer code, unrolled by 9, Superscalar x3"
.globl unroll8x4_descr
	.align 32
	.type	unroll8x4_descr, @object
	.size	unroll8x4_descr, 55
unroll8x4_descr:
	.string	"unroll8x4: Pointer code, unrolled by 8, Superscalar x4"
.globl unroll8x8_descr
	.align 32
	.type	unroll8x8_descr, @object
	.size	unroll8x8_descr, 55
unroll8x8_descr:
	.string	"unroll8x8: Pointer code, unrolled by 8, Superscalar x8"
.globl combine7_descr
	.align 32
	.type	combine7_descr, @object
	.size	combine7_descr, 61
combine7_descr:
	.string	"combine7: Array code, unrolled by 2, different associativity"
.globl unroll3aa_descr
	.align 32
	.type	unroll3aa_descr, @object
	.size	unroll3aa_descr, 62
unroll3aa_descr:
	.string	"unroll3aa: Array code, unrolled by 3, Different Associativity"
.globl unroll4aa_descr
	.align 32
	.type	unroll4aa_descr, @object
	.size	unroll4aa_descr, 62
unroll4aa_descr:
	.string	"unroll4aa: Array code, unrolled by 4, Different Associativity"
.globl unroll5aa_descr
	.align 32
	.type	unroll5aa_descr, @object
	.size	unroll5aa_descr, 62
unroll5aa_descr:
	.string	"unroll5aa: Array code, unrolled by 5, Different Associativity"
.globl unroll6aa_descr
	.align 32
	.type	unroll6aa_descr, @object
	.size	unroll6aa_descr, 62
unroll6aa_descr:
	.string	"unroll6aa: Array code, unrolled by 6, Different Associativity"
.globl unroll8aa_descr
	.align 32
	.type	unroll8aa_descr, @object
	.size	unroll8aa_descr, 62
unroll8aa_descr:
	.string	"unroll8aa: Array code, unrolled by 8, Different Associativity"
.globl simd_v1_descr
	.align 32
	.type	simd_v1_descr, @object
	.size	simd_v1_descr, 43
simd_v1_descr:
	.string	"simd_v1: SSE code, 1*VSIZE-way parallelism"
.globl simd_v2_descr
	.align 32
	.type	simd_v2_descr, @object
	.size	simd_v2_descr, 43
simd_v2_descr:
	.string	"simd_v2: SSE code, 2*VSIZE-way parallelism"
.globl simd_v4_descr
	.align 32
	.type	simd_v4_descr, @object
	.size	simd_v4_descr, 43
simd_v4_descr:
	.string	"simd_v4: SSE code, 4*VSIZE-way parallelism"
.globl simd_v8_descr
	.align 32
	.type	simd_v8_descr, @object
	.size	simd_v8_descr, 43
simd_v8_descr:
	.string	"simd_v8: SSE code, 8*VSIZE-way parallelism"
.globl simd_v12_descr
	.align 32
	.type	simd_v12_descr, @object
	.size	simd_v12_descr, 45
simd_v12_descr:
	.string	"simd_v12: SSE code, 12*VSIZE-way parallelism"
.globl simd_v2a_descr
	.align 32
	.type	simd_v2a_descr, @object
	.size	simd_v2a_descr, 57
simd_v2a_descr:
	.string	"simd_v2a: SSE code, 2*VSIZE-way parallelism, reassociate"
.globl simd_v4a_descr
	.align 32
	.type	simd_v4a_descr, @object
	.size	simd_v4a_descr, 57
simd_v4a_descr:
	.string	"simd_v4a: SSE code, 4*VSIZE-way parallelism, reassociate"
.globl simd_v8a_descr
	.align 32
	.type	simd_v8a_descr, @object
	.size	simd_v8a_descr, 57
simd_v8a_descr:
	.string	"simd_v8a: SSE code, 8*VSIZE-way parallelism, reassociate"
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	3951369912
	.long	1070512209
	.align 8
.LC1:
	.long	1202590843
	.long	1069841121
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC2:
	.long	1065353216
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC3:
	.long	1065353216
	.long	1065353216
	.long	1065353216
	.long	1065353216
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
	.long	.LFB55
	.long	.LFE55-.LFB55
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB55
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB54
	.long	.LFE54-.LFB54
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI1-.LFB54
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB53
	.long	.LFE53-.LFB53
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI5-.LFB53
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI8-.LCFI7
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB52
	.long	.LFE52-.LFB52
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI9-.LFB52
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI10-.LCFI9
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI11-.LCFI10
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI12-.LCFI11
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB51
	.long	.LFE51-.LFB51
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI13-.LFB51
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI14-.LCFI13
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI15-.LCFI14
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI16-.LCFI15
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB50
	.long	.LFE50-.LFB50
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI17-.LFB50
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI18-.LCFI17
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI19-.LCFI18
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI20-.LCFI19
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.long	.LFB49
	.long	.LFE49-.LFB49
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI21-.LFB49
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI22-.LCFI21
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI23-.LCFI22
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI24-.LCFI23
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.long	.LFB48
	.long	.LFE48-.LFB48
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI25-.LFB48
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI26-.LCFI25
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI27-.LCFI26
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI28-.LCFI27
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE15:
.LSFDE17:
	.long	.LEFDE17-.LASFDE17
.LASFDE17:
	.long	.LASFDE17-.Lframe1
	.long	.LFB47
	.long	.LFE47-.LFB47
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI29-.LFB47
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI30-.LCFI29
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI31-.LCFI30
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI32-.LCFI31
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE17:
.LSFDE19:
	.long	.LEFDE19-.LASFDE19
.LASFDE19:
	.long	.LASFDE19-.Lframe1
	.long	.LFB46
	.long	.LFE46-.LFB46
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI33-.LFB46
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI34-.LCFI33
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI35-.LCFI34
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI36-.LCFI35
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI37-.LCFI36
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE19:
.LSFDE21:
	.long	.LEFDE21-.LASFDE21
.LASFDE21:
	.long	.LASFDE21-.Lframe1
	.long	.LFB45
	.long	.LFE45-.LFB45
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI38-.LFB45
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI39-.LCFI38
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI40-.LCFI39
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI41-.LCFI40
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI42-.LCFI41
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE21:
.LSFDE23:
	.long	.LEFDE23-.LASFDE23
.LASFDE23:
	.long	.LASFDE23-.Lframe1
	.long	.LFB44
	.long	.LFE44-.LFB44
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI43-.LFB44
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI44-.LCFI43
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI45-.LCFI44
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI46-.LCFI45
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI47-.LCFI46
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE23:
.LSFDE25:
	.long	.LEFDE25-.LASFDE25
.LASFDE25:
	.long	.LASFDE25-.Lframe1
	.long	.LFB43
	.long	.LFE43-.LFB43
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI48-.LFB43
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI49-.LCFI48
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI50-.LCFI49
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI51-.LCFI50
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI52-.LCFI51
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE25:
.LSFDE27:
	.long	.LEFDE27-.LASFDE27
.LASFDE27:
	.long	.LASFDE27-.Lframe1
	.long	.LFB42
	.long	.LFE42-.LFB42
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI53-.LFB42
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI54-.LCFI53
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI55-.LCFI54
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI56-.LCFI55
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI57-.LCFI56
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE27:
.LSFDE29:
	.long	.LEFDE29-.LASFDE29
.LASFDE29:
	.long	.LASFDE29-.Lframe1
	.long	.LFB41
	.long	.LFE41-.LFB41
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI58-.LFB41
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI59-.LCFI58
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI60-.LCFI59
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI61-.LCFI60
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI62-.LCFI61
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE29:
.LSFDE31:
	.long	.LEFDE31-.LASFDE31
.LASFDE31:
	.long	.LASFDE31-.Lframe1
	.long	.LFB40
	.long	.LFE40-.LFB40
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI63-.LFB40
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI64-.LCFI63
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI65-.LCFI64
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE31:
.LSFDE33:
	.long	.LEFDE33-.LASFDE33
.LASFDE33:
	.long	.LASFDE33-.Lframe1
	.long	.LFB39
	.long	.LFE39-.LFB39
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI66-.LFB39
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI67-.LCFI66
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI68-.LCFI67
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE33:
.LSFDE35:
	.long	.LEFDE35-.LASFDE35
.LASFDE35:
	.long	.LASFDE35-.Lframe1
	.long	.LFB38
	.long	.LFE38-.LFB38
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI69-.LFB38
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI70-.LCFI69
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI71-.LCFI70
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
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
	.long	.LCFI72-.LFB37
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI73-.LCFI72
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI74-.LCFI73
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE37:
.LSFDE39:
	.long	.LEFDE39-.LASFDE39
.LASFDE39:
	.long	.LASFDE39-.Lframe1
	.long	.LFB36
	.long	.LFE36-.LFB36
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI75-.LFB36
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI76-.LCFI75
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI77-.LCFI76
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI78-.LCFI77
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI79-.LCFI78
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE39:
.LSFDE41:
	.long	.LEFDE41-.LASFDE41
.LASFDE41:
	.long	.LASFDE41-.Lframe1
	.long	.LFB35
	.long	.LFE35-.LFB35
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI80-.LFB35
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI81-.LCFI80
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI82-.LCFI81
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI83-.LCFI82
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI84-.LCFI83
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE41:
.LSFDE43:
	.long	.LEFDE43-.LASFDE43
.LASFDE43:
	.long	.LASFDE43-.Lframe1
	.long	.LFB34
	.long	.LFE34-.LFB34
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI85-.LFB34
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI86-.LCFI85
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI87-.LCFI86
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI88-.LCFI87
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI89-.LCFI88
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE43:
.LSFDE45:
	.long	.LEFDE45-.LASFDE45
.LASFDE45:
	.long	.LASFDE45-.Lframe1
	.long	.LFB33
	.long	.LFE33-.LFB33
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI90-.LFB33
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI91-.LCFI90
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI92-.LCFI91
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI93-.LCFI92
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI94-.LCFI93
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE45:
.LSFDE47:
	.long	.LEFDE47-.LASFDE47
.LASFDE47:
	.long	.LASFDE47-.Lframe1
	.long	.LFB32
	.long	.LFE32-.LFB32
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI95-.LFB32
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI96-.LCFI95
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI97-.LCFI96
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI98-.LCFI97
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI99-.LCFI98
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE47:
.LSFDE49:
	.long	.LEFDE49-.LASFDE49
.LASFDE49:
	.long	.LASFDE49-.Lframe1
	.long	.LFB31
	.long	.LFE31-.LFB31
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI100-.LFB31
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI101-.LCFI100
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI102-.LCFI101
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI103-.LCFI102
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI104-.LCFI103
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE49:
.LSFDE51:
	.long	.LEFDE51-.LASFDE51
.LASFDE51:
	.long	.LASFDE51-.Lframe1
	.long	.LFB30
	.long	.LFE30-.LFB30
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI105-.LFB30
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI106-.LCFI105
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI107-.LCFI106
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI108-.LCFI107
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI109-.LCFI108
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE51:
.LSFDE53:
	.long	.LEFDE53-.LASFDE53
.LASFDE53:
	.long	.LASFDE53-.Lframe1
	.long	.LFB29
	.long	.LFE29-.LFB29
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI110-.LFB29
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI111-.LCFI110
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI112-.LCFI111
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI113-.LCFI112
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI114-.LCFI113
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE53:
.LSFDE55:
	.long	.LEFDE55-.LASFDE55
.LASFDE55:
	.long	.LASFDE55-.Lframe1
	.long	.LFB28
	.long	.LFE28-.LFB28
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI115-.LFB28
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI116-.LCFI115
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI117-.LCFI116
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI118-.LCFI117
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI119-.LCFI118
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE55:
.LSFDE57:
	.long	.LEFDE57-.LASFDE57
.LASFDE57:
	.long	.LASFDE57-.Lframe1
	.long	.LFB27
	.long	.LFE27-.LFB27
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI120-.LFB27
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI121-.LCFI120
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI122-.LCFI121
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI123-.LCFI122
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI124-.LCFI123
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE57:
.LSFDE59:
	.long	.LEFDE59-.LASFDE59
.LASFDE59:
	.long	.LASFDE59-.Lframe1
	.long	.LFB26
	.long	.LFE26-.LFB26
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI125-.LFB26
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI126-.LCFI125
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI127-.LCFI126
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI128-.LCFI127
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI129-.LCFI128
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE59:
.LSFDE61:
	.long	.LEFDE61-.LASFDE61
.LASFDE61:
	.long	.LASFDE61-.Lframe1
	.long	.LFB25
	.long	.LFE25-.LFB25
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI130-.LFB25
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI131-.LCFI130
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI132-.LCFI131
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI133-.LCFI132
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI134-.LCFI133
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE61:
.LSFDE63:
	.long	.LEFDE63-.LASFDE63
.LASFDE63:
	.long	.LASFDE63-.Lframe1
	.long	.LFB24
	.long	.LFE24-.LFB24
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI135-.LFB24
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI136-.LCFI135
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI137-.LCFI136
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI138-.LCFI137
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI139-.LCFI138
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE63:
.LSFDE65:
	.long	.LEFDE65-.LASFDE65
.LASFDE65:
	.long	.LASFDE65-.Lframe1
	.long	.LFB23
	.long	.LFE23-.LFB23
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI140-.LFB23
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI141-.LCFI140
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI142-.LCFI141
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI143-.LCFI142
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI144-.LCFI143
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE65:
.LSFDE67:
	.long	.LEFDE67-.LASFDE67
.LASFDE67:
	.long	.LASFDE67-.Lframe1
	.long	.LFB22
	.long	.LFE22-.LFB22
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI145-.LFB22
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI146-.LCFI145
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI147-.LCFI146
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE67:
.LSFDE69:
	.long	.LEFDE69-.LASFDE69
.LASFDE69:
	.long	.LASFDE69-.Lframe1
	.long	.LFB21
	.long	.LFE21-.LFB21
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI148-.LFB21
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI149-.LCFI148
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI150-.LCFI149
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE69:
.LSFDE71:
	.long	.LEFDE71-.LASFDE71
.LASFDE71:
	.long	.LASFDE71-.Lframe1
	.long	.LFB20
	.long	.LFE20-.LFB20
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI151-.LFB20
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI152-.LCFI151
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI153-.LCFI152
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE71:
.LSFDE73:
	.long	.LEFDE73-.LASFDE73
.LASFDE73:
	.long	.LASFDE73-.Lframe1
	.long	.LFB19
	.long	.LFE19-.LFB19
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI154-.LFB19
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI155-.LCFI154
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI156-.LCFI155
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE73:
.LSFDE75:
	.long	.LEFDE75-.LASFDE75
.LASFDE75:
	.long	.LASFDE75-.Lframe1
	.long	.LFB18
	.long	.LFE18-.LFB18
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI157-.LFB18
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI158-.LCFI157
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI159-.LCFI158
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE75:
.LSFDE77:
	.long	.LEFDE77-.LASFDE77
.LASFDE77:
	.long	.LASFDE77-.Lframe1
	.long	.LFB17
	.long	.LFE17-.LFB17
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI160-.LFB17
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI161-.LCFI160
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI162-.LCFI161
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI163-.LCFI162
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI164-.LCFI163
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE77:
.LSFDE79:
	.long	.LEFDE79-.LASFDE79
.LASFDE79:
	.long	.LASFDE79-.Lframe1
	.long	.LFB16
	.long	.LFE16-.LFB16
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI165-.LFB16
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI166-.LCFI165
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI167-.LCFI166
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI168-.LCFI167
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI169-.LCFI168
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE79:
.LSFDE81:
	.long	.LEFDE81-.LASFDE81
.LASFDE81:
	.long	.LASFDE81-.Lframe1
	.long	.LFB15
	.long	.LFE15-.LFB15
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI170-.LFB15
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI171-.LCFI170
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI172-.LCFI171
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI173-.LCFI172
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI174-.LCFI173
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE81:
.LSFDE83:
	.long	.LEFDE83-.LASFDE83
.LASFDE83:
	.long	.LASFDE83-.Lframe1
	.long	.LFB14
	.long	.LFE14-.LFB14
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI175-.LFB14
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI176-.LCFI175
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI177-.LCFI176
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI178-.LCFI177
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI179-.LCFI178
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE83:
.LSFDE85:
	.long	.LEFDE85-.LASFDE85
.LASFDE85:
	.long	.LASFDE85-.Lframe1
	.long	.LFB13
	.long	.LFE13-.LFB13
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI180-.LFB13
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI181-.LCFI180
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI182-.LCFI181
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI183-.LCFI182
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI184-.LCFI183
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE85:
.LSFDE87:
	.long	.LEFDE87-.LASFDE87
.LASFDE87:
	.long	.LASFDE87-.Lframe1
	.long	.LFB12
	.long	.LFE12-.LFB12
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI185-.LFB12
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI186-.LCFI185
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI187-.LCFI186
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI188-.LCFI187
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI189-.LCFI188
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE87:
.LSFDE89:
	.long	.LEFDE89-.LASFDE89
.LASFDE89:
	.long	.LASFDE89-.Lframe1
	.long	.LFB11
	.long	.LFE11-.LFB11
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI190-.LFB11
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI191-.LCFI190
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI192-.LCFI191
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.align 8
.LEFDE89:
.LSFDE91:
	.long	.LEFDE91-.LASFDE91
.LASFDE91:
	.long	.LASFDE91-.Lframe1
	.long	.LFB10
	.long	.LFE10-.LFB10
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI193-.LFB10
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI194-.LCFI193
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI195-.LCFI194
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI196-.LCFI195
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI197-.LCFI196
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE91:
.LSFDE93:
	.long	.LEFDE93-.LASFDE93
.LASFDE93:
	.long	.LASFDE93-.Lframe1
	.long	.LFB9
	.long	.LFE9-.LFB9
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI198-.LFB9
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI199-.LCFI198
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI200-.LCFI199
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI201-.LCFI200
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI202-.LCFI201
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE93:
.LSFDE95:
	.long	.LEFDE95-.LASFDE95
.LASFDE95:
	.long	.LASFDE95-.Lframe1
	.long	.LFB8
	.long	.LFE8-.LFB8
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI203-.LFB8
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI204-.LCFI203
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI205-.LCFI204
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE95:
.LSFDE97:
	.long	.LEFDE97-.LASFDE97
.LASFDE97:
	.long	.LASFDE97-.Lframe1
	.long	.LFB7
	.long	.LFE7-.LFB7
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI206-.LFB7
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI207-.LCFI206
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI208-.LCFI207
	.byte	0xe
	.uleb128 0x20
	.align 8
.LEFDE97:
.LSFDE99:
	.long	.LEFDE99-.LASFDE99
.LASFDE99:
	.long	.LASFDE99-.Lframe1
	.long	.LFB6
	.long	.LFE6-.LFB6
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI209-.LFB6
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI210-.LCFI209
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI211-.LCFI210
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE99:
.LSFDE101:
	.long	.LEFDE101-.LASFDE101
.LASFDE101:
	.long	.LASFDE101-.Lframe1
	.long	.LFB5
	.long	.LFE5-.LFB5
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI212-.LFB5
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI213-.LCFI212
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI214-.LCFI213
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE101:
.LSFDE103:
	.long	.LEFDE103-.LASFDE103
.LASFDE103:
	.long	.LASFDE103-.Lframe1
	.long	.LFB4
	.long	.LFE4-.LFB4
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI215-.LFB4
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI216-.LCFI215
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI217-.LCFI216
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE103:
.LSFDE105:
	.long	.LEFDE105-.LASFDE105
.LASFDE105:
	.long	.LASFDE105-.Lframe1
	.long	.LFB3
	.long	.LFE3-.LFB3
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI218-.LFB3
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI219-.LCFI218
	.byte	0xe
	.uleb128 0x18
	.byte	0x8d
	.uleb128 0x3
	.byte	0x8e
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI220-.LCFI219
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI221-.LCFI220
	.byte	0xe
	.uleb128 0x28
	.byte	0x86
	.uleb128 0x5
	.byte	0x8c
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI222-.LCFI221
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI223-.LCFI222
	.byte	0xe
	.uleb128 0x40
	.byte	0x83
	.uleb128 0x6
	.align 8
.LEFDE105:
.LSFDE107:
	.long	.LEFDE107-.LASFDE107
.LASFDE107:
	.long	.LASFDE107-.Lframe1
	.long	.LFB2
	.long	.LFE2-.LFB2
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI224-.LFB2
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI225-.LCFI224
	.byte	0xe
	.uleb128 0x18
	.byte	0x8c
	.uleb128 0x3
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI226-.LCFI225
	.byte	0xe
	.uleb128 0x20
	.byte	0x86
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI227-.LCFI226
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI228-.LCFI227
	.byte	0xe
	.uleb128 0x40
	.align 8
.LEFDE107:
	.ident	"GCC: (Ubuntu 4.3.2-1ubuntu11) 4.3.2"
	.section	.note.GNU-stack,"",@progbits
