==11426== Memcheck, a memory error detector
==11426== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==11426== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==11426== Command: ./interpret ./Testy/test_lex1
==11426== Parent PID: 11409
==11426== 
==11426== 
==11426== HEAP SUMMARY:
==11426==     in use at exit: 16 bytes in 1 blocks
==11426==   total heap usage: 3 allocs, 2 frees, 1,592 bytes allocated
==11426== 
==11426== LEAK SUMMARY:
==11426==    definitely lost: 0 bytes in 0 blocks
==11426==    indirectly lost: 0 bytes in 0 blocks
==11426==      possibly lost: 0 bytes in 0 blocks
==11426==    still reachable: 16 bytes in 1 blocks
==11426==         suppressed: 0 bytes in 0 blocks
==11426== Rerun with --leak-check=full to see details of leaked memory
==11426== 
==11426== For counts of detected and suppressed errors, rerun with: -v
==11426== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
