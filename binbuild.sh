#!/bin/bash
for f in *.dmp ; do
    a=`basename $f .dmp`
    cut -d'#' -f1 <$f | rdmp -b >$a
    chmod +x $a
done
