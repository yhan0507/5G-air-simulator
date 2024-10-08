# import from LTE-SIM
#
# Copyright (c) 2010
#
# This file is part of LTE-Sim
# LTE-Sim is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 3 as
# published by the Free Software Foundation;
#
# LTE-Sim is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with LTE-Sim; if not, see <http://www.gnu.org/licenses/>.
#
# Author: Mauricio Iturralde <mauricio.iturralde@irit.fr, mauro@miturralde.com>

# params 1 MINUSERS, 2 MAXUSERS, 3 INTERVAL, 4 FILENAME, 5 FILE, 6 NUMSIM, 7 TYPE, 8 FILE_NAME_PLOT,

# OUTPUT FILE NAME
FILE=$5
#SIMULATIONS NUMBER
NUMSIM=$6
# SIMULATION TYPE NAME
FILENAME=$4
# NUMBER OF CELLS
CELS=7
#Number of UE's
NBUE=$1
# variables for values
COUNT=1
TOTALNAME=""

until [ $NBUE -gt $2 ]; do
	# bash until loop
	# GRAPHIC FOR PROPORTIONAL FAIRNESS
	until [ $COUNT -gt $NUMSIM ]; do
		TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_PF_"$NBUE"U"$CELS"C"".sim"
		grep "RX "$7 $TOTALNAME | grep "D" | awk '{print $14}' >tmp

		./compute_delay.sh tmp >>temporal
		rm tmp
		let COUNT=COUNT+1
	done
	./compute_average.sh temporal | awk '{print "'$NBUE' "$1}' >>PF_Y1_$8_$7.dat
	COUNT=1
	rm temporal

	#GRAPHIC FOR M-LWDF
	until [ $COUNT -gt $NUMSIM ]; do
		TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_MLWDF_"$NBUE"U"$CELS"C"".sim"

		grep "RX "$7 $TOTALNAME | grep "D" | awk '{print $14}' >tmp
		./compute_delay.sh tmp >>temporal
		rm tmp
		let COUNT=COUNT+1
	done
	./compute_average.sh temporal | awk '{print "'$NBUE' "$1}' >>MLWDF_Y1_$8_$7.dat
	COUNT=1
	rm temporal

	#GRAPHIC FOR EXPPF

	until [ $COUNT -gt $NUMSIM ]; do
		TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_EXPPF_"$NBUE"U"$CELS"C"".sim"

		grep "RX "$7 $TOTALNAME | grep "D" | awk '{print $14}' >tmp
		./compute_delay.sh tmp >>temporal
		rm tmp
		let COUNT=COUNT+1
	done
	./compute_average.sh temporal | awk '{print "'$NBUE' "$1}' >>EXPPF_Y1_$8_$7.dat
	COUNT=1
	rm temporal

	#GRAPHIC FOR FLS
	until [ $COUNT -gt $NUMSIM ]; do
		TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_FLS_"$NBUE"U"$CELS"C"".sim"

		grep "RX "$7 $TOTALNAME | grep "D" | awk '{print $14}' >tmp
		./compute_delay.sh tmp >>temporal
		rm tmp
		let COUNT=COUNT+1
	done
	./compute_average.sh temporal | awk '{print "'$NBUE' "$1}' >>FLS_Y1_$8_$7.dat
	COUNT=1
	rm temporal

	#GRAPHIC FOR EXPRULE
	until [ $COUNT -gt $NUMSIM ]; do
		TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_EXPRULE_"$NBUE"U"$CELS"C"".sim"

		grep "RX "$7 $TOTALNAME | grep "D" | awk '{print $14}' >tmp
		./compute_delay.sh tmp >>temporal
		rm tmp
		let COUNT=COUNT+1
	done
	./compute_average.sh temporal | awk '{print "'$NBUE' "$1}' >>EXPRULE_Y1_$8_$7.dat
	COUNT=1
	rm temporal

	#GRAPHIC FOR LOGRULE
	until [ $COUNT -gt $NUMSIM ]; do
		TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_LOGRULE_"$NBUE"U"$CELS"C"".sim"

		grep "RX "$7 $TOTALNAME | grep "D" | awk '{print $14}' >tmp
		./compute_delay.sh tmp >>temporal
		rm tmp
		let COUNT=COUNT+1
	done
	./compute_average.sh temporal | awk '{print "'$NBUE' "$1}' >>LOGRULE_Y1_$8_$7.dat
	COUNT=1
	rm temporal

	until [ $COUNT -gt $NUMSIM ]; do
		TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_MT_"$NBUE"U"$CELS"C"".sim"

		grep "RX "$7 $TOTALNAME | grep "D" | awk '{print $14}' >tmp
		./compute_delay.sh tmp >>temporal
		rm tmp
		let COUNT=COUNT+1
	done
	./compute_average.sh temporal | awk '{print "'$NBUE' "$1}' >>MT_Y1_$8_$7.dat
	COUNT=1
	rm temporal

	until [ $COUNT -gt $NUMSIM ]; do
		TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_RR_"$NBUE"U"$CELS"C"".sim"

		grep "RX "$7 $TOTALNAME | grep "D" | awk '{print $14}' >tmp
		./compute_delay.sh tmp >>temporal
		rm tmp
		let COUNT=COUNT+1
	done
	./compute_average.sh temporal | awk '{print "'$NBUE' "$1}' >>RR_Y1_$8_$7.dat
	COUNT=1
	rm temporal

	let NBUE=NBUE+$3
done

echo PF >>results_$8_$7.ods
echo Users Value >>results_$8_$7.ods
grep " " PF_Y1_$8_$7.dat >>results_$8_$7.ods

echo MLWDF >>results_$8_$7.ods
echo Users Value >>results_$8_$7.ods
grep " " MLWDF_Y1_$8_$7.dat >>results_$8_$7.ods

echo EXP-PF >>results_$8_$7.ods
echo Users Value >>results_$8_$7.ods
grep " " EXPPF_Y1_$8_$7.dat >>results_$8_$7.ods

echo FLS >>results_$8_$7.ods
echo Users Value >>results_$8_$7.ods
grep " " FLS_Y1_$8_$7.dat >>results_$8_$7.ods

echo EXPRULE >>results_$8_$7.ods
echo Users Value >>results_$8_$7.ods
grep " " EXPRULE_Y1_$8_$7.dat >>results_$8_$7.ods

echo LOGRULE >>results_$8_$7.ods
echo Users Value >>results_$8_$7.ods
grep " " LOGRULE_Y1_$8_$7.dat >>results_$8_$7.ods

echo MT >>results_$8_$7.ods
echo Users Value >>results_$8_$7.ods
grep " " MT_Y1_$8_$7.dat >>results_$8_$7.ods

echo RR >>results_$8_$7.ods
echo Users Value >>results_$8_$7.ods
grep " " RR_Y1_$8_$7.dat >>results_$8_$7.ods


./Graph1.sh "Delays (s)" "Number of user" $7 $7_$8 PF_Y1_$8_$7.dat MLWDF_Y1_$8_$7.dat EXPPF_Y1_$8_$7.dat QLE_Y1_$8_$7.dat 

rm PF_Y1_$8_$7.dat
rm MLWDF_Y1_$8_$7.dat
rm EXPPF_Y1_$8_$7.dat

rm FLS_Y1_$8_$7.dat
rm EXPRULE_Y1_$8_$7.dat
rm LOGRULE_Y1_$8_$7.dat
rm MT_Y1_$8_$7.dat
rm RR_Y1_$8_$7.dat


echo Delay $7 REPORT FINISHED!!
