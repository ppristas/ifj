#!/bin/bash


  testing_files=$(ls -l $1| grep "\.test" | awk '{print $9}'| sed 's|*.\.||' | awk -F '\.' '{print $1}');
  testing_file_count=$(ls -l $1| grep "\.test" | awk '{print $9}'| sed 's|*.\.||' | wc -l );
  counter=1;
  for i in $testing_files; do
    echo "________________________________________________"
    echo "  $counter.TEST: $1$i"
    echo "________________________________________________"
    echo ""
    ./interpret "$1$i.test" 2>/dev/null 1>$1/$i.out;
    return_value=$(echo $?);
    k=$(cat $1$i.return)
    echo "------------RETURN VALUE CHECK------------------"
    if [[ $k -eq $return_value ]]; then
      echo "    Return value of $i.test is OK"
    else
      echo "ERROR"
      echo "    Actual return value: $return_value"
      echo "    Expected return value: $k"
    fi

    echo "------------------------------------------------"
    echo ""
    echo "------------OUTPUT CHECK------------------------"
    if [[ $k -eq 0 ]]; then
      diffe=$(diff $1$i.out $1$i.output)
      difference=$(diff $1$i.out $1$i.output | wc -l);
      if [[ "$difference" = "0" ]]; then
        echo "    OUTPUT IS OK"
      else
        echo $diffe
      fi
    fi
    echo "------------------------------------------------"
    echo ""

    valgrind --log-file=$1check.k ./interpret "$1$i.test" 2>/dev/null
    valgr_vypis=$(cat $1check.k | sed 's/^.*[=]/     |/g' | grep bytes);
    valgr_err=$(cat $1check.k | grep "ERROR SUMMARY" | awk -F " " '{print $4}');
    valgr_cont=$(cat $1check.k | grep "ERROR SUMMARY" | awk -F " " '{print $7}');
    echo "------------MEMMORY CHECK-----------------------"
    if [[ $valgr_err -eq $valgr_cont ]]; then
      echo "    NO MEMMORY ERRORS IN $1$i"
      echo ""
    else
      echo "ERRORS FOUND IN $1$i"
      echo "    $valgr_vypis"
    fi

    valgr_byte1=$(cat $1check.k | sed 's/^.*[=]//g'| grep bytes | head -n 1 | awk -F " " '{print $5}');
    valgr_byte2=$(cat $1check.k | sed 's/^.*[=]//g'| grep bytes | head -n 1 | awk -F " " '{print $8}')
    valgr_line=$(cat $1check.k | sed 's/^.*[=]/     |/g' | grep bytes | wc -l)
    if [[ $valgr_line -eq 2 ]]; then
      echo "    NO HEAP ERRORS IN $1$i"
    else
      echo "ERRORS FOUND IN $1$i"
      echo "$valgr_vypis"
    fi
    echo "------------------------------------------------"

    if [[ $counter -ne $testing_file_count ]]; then
      echo "Another test? Y/N"
      read next_file;
    fi;
    if [ "$next_file" != "Y" -a "$next_file" != "y" ]
      then
        exit 1;
    fi;
    echo ""

    ((counter+=1));

  done;
    remove=$(ls -l $1 | grep "*.out" | wc -l)
    if [[ "$remove" -ne "0" ]]; then
      rm -r $1*.out
    fi
