
program:     file format elf64-x86-64


Disassembly of section .init:

0000000000401000 <_init>:
  401000:	f3 0f 1e fa          	endbr64
  401004:	48 83 ec 08          	sub    $0x8,%rsp
  401008:	48 8b 05 e1 2f 00 00 	mov    0x2fe1(%rip),%rax        # 403ff0 <__gmon_start__>
  40100f:	48 85 c0             	test   %rax,%rax
  401012:	74 02                	je     401016 <_init+0x16>
  401014:	ff d0                	call   *%rax
  401016:	48 83 c4 08          	add    $0x8,%rsp
  40101a:	c3                   	ret

Disassembly of section .plt:

0000000000401020 <.plt>:
  401020:	ff 35 e2 2f 00 00    	push   0x2fe2(%rip)        # 404008 <_GLOBAL_OFFSET_TABLE_+0x8>
  401026:	ff 25 e4 2f 00 00    	jmp    *0x2fe4(%rip)        # 404010 <_GLOBAL_OFFSET_TABLE_+0x10>
  40102c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000401030 <getenv@plt>:
  401030:	ff 25 e2 2f 00 00    	jmp    *0x2fe2(%rip)        # 404018 <getenv@GLIBC_2.2.5>
  401036:	68 00 00 00 00       	push   $0x0
  40103b:	e9 e0 ff ff ff       	jmp    401020 <.plt>

0000000000401040 <free@plt>:
  401040:	ff 25 da 2f 00 00    	jmp    *0x2fda(%rip)        # 404020 <free@GLIBC_2.2.5>
  401046:	68 01 00 00 00       	push   $0x1
  40104b:	e9 d0 ff ff ff       	jmp    401020 <.plt>

0000000000401050 <puts@plt>:
  401050:	ff 25 d2 2f 00 00    	jmp    *0x2fd2(%rip)        # 404028 <puts@GLIBC_2.2.5>
  401056:	68 02 00 00 00       	push   $0x2
  40105b:	e9 c0 ff ff ff       	jmp    401020 <.plt>

0000000000401060 <strlen@plt>:
  401060:	ff 25 ca 2f 00 00    	jmp    *0x2fca(%rip)        # 404030 <strlen@GLIBC_2.2.5>
  401066:	68 03 00 00 00       	push   $0x3
  40106b:	e9 b0 ff ff ff       	jmp    401020 <.plt>

0000000000401070 <printf@plt>:
  401070:	ff 25 c2 2f 00 00    	jmp    *0x2fc2(%rip)        # 404038 <printf@GLIBC_2.2.5>
  401076:	68 04 00 00 00       	push   $0x4
  40107b:	e9 a0 ff ff ff       	jmp    401020 <.plt>

0000000000401080 <strcmp@plt>:
  401080:	ff 25 ba 2f 00 00    	jmp    *0x2fba(%rip)        # 404040 <strcmp@GLIBC_2.2.5>
  401086:	68 05 00 00 00       	push   $0x5
  40108b:	e9 90 ff ff ff       	jmp    401020 <.plt>

0000000000401090 <malloc@plt>:
  401090:	ff 25 b2 2f 00 00    	jmp    *0x2fb2(%rip)        # 404048 <malloc@GLIBC_2.2.5>
  401096:	68 06 00 00 00       	push   $0x6
  40109b:	e9 80 ff ff ff       	jmp    401020 <.plt>

00000000004010a0 <fflush@plt>:
  4010a0:	ff 25 aa 2f 00 00    	jmp    *0x2faa(%rip)        # 404050 <fflush@GLIBC_2.2.5>
  4010a6:	68 07 00 00 00       	push   $0x7
  4010ab:	e9 70 ff ff ff       	jmp    401020 <.plt>

00000000004010b0 <sleep@plt>:
  4010b0:	ff 25 a2 2f 00 00    	jmp    *0x2fa2(%rip)        # 404058 <sleep@GLIBC_2.2.5>
  4010b6:	68 08 00 00 00       	push   $0x8
  4010bb:	e9 60 ff ff ff       	jmp    401020 <.plt>

Disassembly of section .text:

00000000004010c0 <_start>:
  4010c0:	f3 0f 1e fa          	endbr64
  4010c4:	31 ed                	xor    %ebp,%ebp
  4010c6:	49 89 d1             	mov    %rdx,%r9
  4010c9:	5e                   	pop    %rsi
  4010ca:	48 89 e2             	mov    %rsp,%rdx
  4010cd:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  4010d1:	50                   	push   %rax
  4010d2:	54                   	push   %rsp
  4010d3:	45 31 c0             	xor    %r8d,%r8d
  4010d6:	31 c9                	xor    %ecx,%ecx
  4010d8:	48 c7 c7 2c 12 40 00 	mov    $0x40122c,%rdi
  4010df:	ff 15 fb 2e 00 00    	call   *0x2efb(%rip)        # 403fe0 <__libc_start_main@GLIBC_2.34>
  4010e5:	f4                   	hlt
  4010e6:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
  4010ed:	00 00 00 

00000000004010f0 <_dl_relocate_static_pie>:
  4010f0:	f3 0f 1e fa          	endbr64
  4010f4:	c3                   	ret
  4010f5:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
  4010fc:	00 00 00 
  4010ff:	90                   	nop

0000000000401100 <deregister_tm_clones>:
  401100:	48 8d 3d 61 2f 00 00 	lea    0x2f61(%rip),%rdi        # 404068 <stdout@GLIBC_2.2.5>
  401107:	48 8d 05 5a 2f 00 00 	lea    0x2f5a(%rip),%rax        # 404068 <stdout@GLIBC_2.2.5>
  40110e:	48 39 f8             	cmp    %rdi,%rax
  401111:	74 15                	je     401128 <deregister_tm_clones+0x28>
  401113:	48 8b 05 ce 2e 00 00 	mov    0x2ece(%rip),%rax        # 403fe8 <_ITM_deregisterTMCloneTable>
  40111a:	48 85 c0             	test   %rax,%rax
  40111d:	74 09                	je     401128 <deregister_tm_clones+0x28>
  40111f:	ff e0                	jmp    *%rax
  401121:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
  401128:	c3                   	ret
  401129:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000401130 <register_tm_clones>:
  401130:	48 8d 3d 31 2f 00 00 	lea    0x2f31(%rip),%rdi        # 404068 <stdout@GLIBC_2.2.5>
  401137:	48 8d 35 2a 2f 00 00 	lea    0x2f2a(%rip),%rsi        # 404068 <stdout@GLIBC_2.2.5>
  40113e:	48 29 fe             	sub    %rdi,%rsi
  401141:	48 89 f0             	mov    %rsi,%rax
  401144:	48 c1 ee 3f          	shr    $0x3f,%rsi
  401148:	48 c1 f8 03          	sar    $0x3,%rax
  40114c:	48 01 c6             	add    %rax,%rsi
  40114f:	48 d1 fe             	sar    $1,%rsi
  401152:	74 14                	je     401168 <register_tm_clones+0x38>
  401154:	48 8b 05 9d 2e 00 00 	mov    0x2e9d(%rip),%rax        # 403ff8 <_ITM_registerTMCloneTable>
  40115b:	48 85 c0             	test   %rax,%rax
  40115e:	74 08                	je     401168 <register_tm_clones+0x38>
  401160:	ff e0                	jmp    *%rax
  401162:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
  401168:	c3                   	ret
  401169:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000401170 <__do_global_dtors_aux>:
  401170:	f3 0f 1e fa          	endbr64
  401174:	80 3d f5 2e 00 00 00 	cmpb   $0x0,0x2ef5(%rip)        # 404070 <completed.0>
  40117b:	75 13                	jne    401190 <__do_global_dtors_aux+0x20>
  40117d:	55                   	push   %rbp
  40117e:	48 89 e5             	mov    %rsp,%rbp
  401181:	e8 7a ff ff ff       	call   401100 <deregister_tm_clones>
  401186:	c6 05 e3 2e 00 00 01 	movb   $0x1,0x2ee3(%rip)        # 404070 <completed.0>
  40118d:	5d                   	pop    %rbp
  40118e:	c3                   	ret
  40118f:	90                   	nop
  401190:	c3                   	ret
  401191:	66 66 2e 0f 1f 84 00 	data16 cs nopw 0x0(%rax,%rax,1)
  401198:	00 00 00 00 
  40119c:	0f 1f 40 00          	nopl   0x0(%rax)

00000000004011a0 <frame_dummy>:
  4011a0:	f3 0f 1e fa          	endbr64
  4011a4:	eb 8a                	jmp    401130 <register_tm_clones>

00000000004011a6 <foo>:
  4011a6:	55                   	push   %rbp
  4011a7:	48 89 e5             	mov    %rsp,%rbp
  4011aa:	48 83 ec 20          	sub    $0x20,%rsp
  4011ae:	bf 7a 00 00 00       	mov    $0x7a,%edi
  4011b3:	e8 d8 fe ff ff       	call   401090 <malloc@plt>
  4011b8:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
  4011bc:	bf 9a 20 40 00       	mov    $0x40209a,%edi
  4011c1:	e8 6a fe ff ff       	call   401030 <getenv@plt>
  4011c6:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
  4011ca:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  4011ce:	48 89 c7             	mov    %rax,%rdi
  4011d1:	e8 8a fe ff ff       	call   401060 <strlen@plt>
  4011d6:	48 89 45 e0          	mov    %rax,-0x20(%rbp)
  4011da:	c7 45 fc 00 00 00 00 	movl   $0x0,-0x4(%rbp)
  4011e1:	eb 3b                	jmp    40121e <foo+0x78>
  4011e3:	8b 45 fc             	mov    -0x4(%rbp),%eax
  4011e6:	48 98                	cltq
  4011e8:	0f b6 80 20 20 40 00 	movzbl 0x402020(%rax),%eax
  4011ef:	89 c6                	mov    %eax,%esi
  4011f1:	8b 45 fc             	mov    -0x4(%rbp),%eax
  4011f4:	48 98                	cltq
  4011f6:	ba 00 00 00 00       	mov    $0x0,%edx
  4011fb:	48 f7 75 e0          	divq   -0x20(%rbp)
  4011ff:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401203:	48 01 d0             	add    %rdx,%rax
  401206:	0f b6 10             	movzbl (%rax),%edx
  401209:	8b 45 fc             	mov    -0x4(%rbp),%eax
  40120c:	48 63 c8             	movslq %eax,%rcx
  40120f:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  401213:	48 01 c8             	add    %rcx,%rax
  401216:	31 f2                	xor    %esi,%edx
  401218:	88 10                	mov    %dl,(%rax)
  40121a:	83 45 fc 01          	addl   $0x1,-0x4(%rbp)
  40121e:	8b 45 fc             	mov    -0x4(%rbp),%eax
  401221:	83 f8 79             	cmp    $0x79,%eax
  401224:	76 bd                	jbe    4011e3 <foo+0x3d>
  401226:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  40122a:	c9                   	leave
  40122b:	c3                   	ret

000000000040122c <main>:
  40122c:	55                   	push   %rbp
  40122d:	48 89 e5             	mov    %rsp,%rbp
  401230:	48 83 ec 10          	sub    $0x10,%rsp
  401234:	89 7d fc             	mov    %edi,-0x4(%rbp)
  401237:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  40123b:	b8 00 00 00 00       	mov    $0x0,%eax
  401240:	e8 15 00 00 00       	call   40125a <p>
  401245:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  401249:	48 83 c0 08          	add    $0x8,%rax
  40124d:	48 8b 00             	mov    (%rax),%rax
  401250:	48 89 c7             	mov    %rax,%rdi
  401253:	e8 9f 00 00 00       	call   4012f7 <v>
  401258:	c9                   	leave
  401259:	c3                   	ret

000000000040125a <p>:
  40125a:	55                   	push   %rbp
  40125b:	48 89 e5             	mov    %rsp,%rbp
  40125e:	bf aa 20 40 00       	mov    $0x4020aa,%edi
  401263:	b8 00 00 00 00       	mov    $0x0,%eax
  401268:	e8 03 fe ff ff       	call   401070 <printf@plt>
  40126d:	48 8b 05 f4 2d 00 00 	mov    0x2df4(%rip),%rax        # 404068 <stdout@GLIBC_2.2.5>
  401274:	48 89 c7             	mov    %rax,%rdi
  401277:	e8 24 fe ff ff       	call   4010a0 <fflush@plt>
  40127c:	bf 01 00 00 00       	mov    $0x1,%edi
  401281:	e8 2a fe ff ff       	call   4010b0 <sleep@plt>
  401286:	bf c7 20 40 00       	mov    $0x4020c7,%edi
  40128b:	e8 c0 fd ff ff       	call   401050 <puts@plt>
  401290:	bf d8 20 40 00       	mov    $0x4020d8,%edi
  401295:	b8 00 00 00 00       	mov    $0x0,%eax
  40129a:	e8 d1 fd ff ff       	call   401070 <printf@plt>
  40129f:	48 8b 05 c2 2d 00 00 	mov    0x2dc2(%rip),%rax        # 404068 <stdout@GLIBC_2.2.5>
  4012a6:	48 89 c7             	mov    %rax,%rdi
  4012a9:	e8 f2 fd ff ff       	call   4010a0 <fflush@plt>
  4012ae:	bf 01 00 00 00       	mov    $0x1,%edi
  4012b3:	e8 f8 fd ff ff       	call   4010b0 <sleep@plt>
  4012b8:	bf c7 20 40 00       	mov    $0x4020c7,%edi
  4012bd:	e8 8e fd ff ff       	call   401050 <puts@plt>
  4012c2:	bf 08 21 40 00       	mov    $0x402108,%edi
  4012c7:	b8 00 00 00 00       	mov    $0x0,%eax
  4012cc:	e8 9f fd ff ff       	call   401070 <printf@plt>
  4012d1:	48 8b 05 90 2d 00 00 	mov    0x2d90(%rip),%rax        # 404068 <stdout@GLIBC_2.2.5>
  4012d8:	48 89 c7             	mov    %rax,%rdi
  4012db:	e8 c0 fd ff ff       	call   4010a0 <fflush@plt>
  4012e0:	bf 01 00 00 00       	mov    $0x1,%edi
  4012e5:	e8 c6 fd ff ff       	call   4010b0 <sleep@plt>
  4012ea:	bf 44 21 40 00       	mov    $0x402144,%edi
  4012ef:	e8 5c fd ff ff       	call   401050 <puts@plt>
  4012f4:	90                   	nop
  4012f5:	5d                   	pop    %rbp
  4012f6:	c3                   	ret

00000000004012f7 <v>:
  4012f7:	55                   	push   %rbp
  4012f8:	48 89 e5             	mov    %rsp,%rbp
  4012fb:	48 83 ec 20          	sub    $0x20,%rsp
  4012ff:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  401303:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401307:	be a3 20 40 00       	mov    $0x4020a3,%esi
  40130c:	48 89 c7             	mov    %rax,%rdi
  40130f:	e8 6c fd ff ff       	call   401080 <strcmp@plt>
  401314:	85 c0                	test   %eax,%eax
  401316:	75 75                	jne    40138d <v+0x96>
  401318:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  40131c:	48 89 c6             	mov    %rax,%rsi
  40131f:	bf 58 21 40 00       	mov    $0x402158,%edi
  401324:	b8 00 00 00 00       	mov    $0x0,%eax
  401329:	e8 42 fd ff ff       	call   401070 <printf@plt>
  40132e:	48 8b 05 33 2d 00 00 	mov    0x2d33(%rip),%rax        # 404068 <stdout@GLIBC_2.2.5>
  401335:	48 89 c7             	mov    %rax,%rdi
  401338:	e8 63 fd ff ff       	call   4010a0 <fflush@plt>
  40133d:	bf 01 00 00 00       	mov    $0x1,%edi
  401342:	e8 69 fd ff ff       	call   4010b0 <sleep@plt>
  401347:	bf c7 20 40 00       	mov    $0x4020c7,%edi
  40134c:	e8 ff fc ff ff       	call   401050 <puts@plt>
  401351:	b8 00 00 00 00       	mov    $0x0,%eax
  401356:	e8 4b fe ff ff       	call   4011a6 <foo>
  40135b:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
  40135f:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401363:	48 89 c7             	mov    %rax,%rdi
  401366:	e8 d5 fc ff ff       	call   401040 <free@plt>
  40136b:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  40136f:	0f b6 00             	movzbl (%rax),%eax
  401372:	0f be c0             	movsbl %al,%eax
  401375:	89 c6                	mov    %eax,%esi
  401377:	bf 9c 21 40 00       	mov    $0x40219c,%edi
  40137c:	b8 00 00 00 00       	mov    $0x0,%eax
  401381:	e8 ea fc ff ff       	call   401070 <printf@plt>
  401386:	b8 00 00 00 00       	mov    $0x0,%eax
  40138b:	eb 05                	jmp    401392 <v+0x9b>
  40138d:	b8 01 00 00 00       	mov    $0x1,%eax
  401392:	c9                   	leave
  401393:	c3                   	ret

Disassembly of section .fini:

0000000000401394 <_fini>:
  401394:	f3 0f 1e fa          	endbr64
  401398:	48 83 ec 08          	sub    $0x8,%rsp
  40139c:	48 83 c4 08          	add    $0x8,%rsp
  4013a0:	c3                   	ret
