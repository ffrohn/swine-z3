folder=$1
shift
../build/swine-z3 --version
for f in $(find $folder -name '*.smt2')
do
    echo $f
    timeout 10 ../build/swine-z3 $@ $f
done
