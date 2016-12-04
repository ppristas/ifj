==7469== Memcheck, a memory error detector
==7469== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==7469== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==7469== Command: ./interpret ./Testy/SYN_TESTS/test_syn1.test
==7469== Parent PID: 7453
==7469== 
==7469== Invalid read of size 8
==7469==    at 0x40622A: after_class_scnd (parser.c:1590)
==7469==    by 0x405ECA: class_scnd (parser.c:1485)
==7469==    by 0x405D2A: prog_scnd (parser.c:1431)
==7469==    by 0x405D75: prog_scnd (parser.c:1444)
==7469==    by 0x402CFF: parser (parser.c:146)
==7469==    by 0x402557: main (main.c:82)
==7469==  Address 0x520dd00 is 0 bytes inside a block of size 56 free'd
==7469==    at 0x4C2EDEB: free (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==7469==    by 0x40A6A8: clearAll (cleaner.c:123)
==7469==    by 0x40D15B: catch_index (preced.c:245)
==7469==    by 0x40DD50: expresion_parser (preced.c:618)
==7469==    by 0x40A006: is_function_call_or_ass (parser.c:3197)
==7469==    by 0x406EBA: main_body_scnd (parser.c:1942)
==7469==    by 0x40621C: after_class_scnd (parser.c:1589)
==7469==    by 0x405ECA: class_scnd (parser.c:1485)
==7469==    by 0x405D2A: prog_scnd (parser.c:1431)
==7469==    by 0x405D75: prog_scnd (parser.c:1444)
==7469==    by 0x402CFF: parser (parser.c:146)
==7469==    by 0x402557: main (main.c:82)
==7469==  Block was alloc'd at
==7469==    at 0x4C2DB8F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==7469==    by 0x40A4FD: mymalloc (cleaner.c:42)
==7469==    by 0x40C016: sym_function_init (ial.c:585)
==7469==    by 0x4032D8: after_class (parser.c:295)
==7469==    by 0x403066: class (parser.c:238)
==7469==    by 0x402D81: prog (parser.c:163)
==7469==    by 0x402DEB: prog (parser.c:178)
==7469==    by 0x402BF3: parser (parser.c:118)
==7469==    by 0x402557: main (main.c:82)
==7469== 
==7469== Invalid write of size 4
==7469==    at 0x406233: after_class_scnd (parser.c:1590)
==7469==    by 0x405ECA: class_scnd (parser.c:1485)
==7469==    by 0x405D2A: prog_scnd (parser.c:1431)
==7469==    by 0x405D75: prog_scnd (parser.c:1444)
==7469==    by 0x402CFF: parser (parser.c:146)
==7469==    by 0x402557: main (main.c:82)
==7469==  Address 0x520df68 is 40 bytes inside a block of size 48 free'd
==7469==    at 0x4C2EDEB: free (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==7469==    by 0x40A6A8: clearAll (cleaner.c:123)
==7469==    by 0x40D15B: catch_index (preced.c:245)
==7469==    by 0x40DD50: expresion_parser (preced.c:618)
==7469==    by 0x40A006: is_function_call_or_ass (parser.c:3197)
==7469==    by 0x406EBA: main_body_scnd (parser.c:1942)
==7469==    by 0x40621C: after_class_scnd (parser.c:1589)
==7469==    by 0x405ECA: class_scnd (parser.c:1485)
==7469==    by 0x405D2A: prog_scnd (parser.c:1431)
==7469==    by 0x405D75: prog_scnd (parser.c:1444)
==7469==    by 0x402CFF: parser (parser.c:146)
==7469==    by 0x402557: main (main.c:82)
==7469==  Block was alloc'd at
==7469==    at 0x4C2DB8F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==7469==    by 0x40A4FD: mymalloc (cleaner.c:42)
==7469==    by 0x40C157: sym_function_init (ial.c:611)
==7469==    by 0x4032D8: after_class (parser.c:295)
==7469==    by 0x403066: class (parser.c:238)
==7469==    by 0x402D81: prog (parser.c:163)
==7469==    by 0x402DEB: prog (parser.c:178)
==7469==    by 0x402BF3: parser (parser.c:118)
==7469==    by 0x402557: main (main.c:82)
==7469== 
==7469== 
==7469== HEAP SUMMARY:
==7469==     in use at exit: 0 bytes in 0 blocks
==7469==   total heap usage: 497 allocs, 497 frees, 29,011 bytes allocated
==7469== 
==7469== All heap blocks were freed -- no leaks are possible
==7469== 
==7469== For counts of detected and suppressed errors, rerun with: -v
==7469== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
