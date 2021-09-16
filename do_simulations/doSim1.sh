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

# Single Cell With Interference

# OUTPUT FILE NAME
FILE="Simulation"
# Number of simulations
NUMSIM=10
# SIMULATION TYPE NAME 
FILENAME="Multi"
COUNT=1
# NUMBER OF CELLS
CELS=7
TOTALNAME=""

# Start users
MINUSERS=10
# Interval between users
INTERVAL=10
# Max of users
MAXUSERS=60

# params of %G NR Simluation MULTICEL

# Radius in Km
RADIUS=1
# Number of UE's
NBUE=1
# Number of Voip Flows
NBVOIP=1
# Number of Video
NBVIDEO=1
# Number of Best Effort Flows
NBBE=0
# Number of CBR flows
NBCBR=1
# FDD or TDD
FRAME_STRUCT=1
# User speed 3, 30, 120
SPEED=30
# Set MaxDelay
MAXDELAY=0.1
# 128, 242, 440, 1000, 3000, 5000, 7000, 8000, 17000kbps
VIDEOBITRATE=440

NBUE=$MINUSERS

#		  ./5G-air-simulator SingleCellWithI nCell r nUE nVoip nVid nBE nCBR sched frStr speed maxD vidRate (seed)
#     --> ./5G-air-simulator SingleCellWithI 7 1 1 0 0 1 0 1 1 3 0.1 128

until [ $NBUE -gt $MAXUSERS ]; do

	# PF
	until [ $COUNT -gt $NUMSIM ]; do
		TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_PF_"$NBUE"U"$CELS"C"".sim"
		../5G-air-simulator SingleCellWithI $CELS $RADIUS $NBUE $NBVOIP $NBVIDEO $NBBE $NBCBR 1 $FRAME_STRUCT $SPEED $MAXDELAY $VIDEOBITRATE >$TOTALNAME
		echo FILE $TOTALNAME CREATED!
		let COUNT=COUNT+1 
	done
	COUNT=1

	# MLWDF
	until [ $COUNT -gt $NUMSIM ]; do
		TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_MLWDF_"$NBUE"U"$CELS"C"".sim"
		../5G-air-simulator SingleCellWithI $CELS $RADIUS $NBUE $NBVOIP $NBVIDEO $NBBE $NBCBR 2 $FRAME_STRUCT $SPEED $MAXDELAY $VIDEOBITRATE >$TOTALNAME
		echo FILE $TOTALNAME CREATED!
		let COUNT=COUNT+1
	done
	COUNT=1

	# EXPPF
	until [ $COUNT -gt $NUMSIM ]; do
		TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_EXPPF_"$NBUE"U"$CELS"C"".sim"
		../5G-air-simulator SingleCellWithI $CELS $RADIUS $NBUE $NBVOIP $NBVIDEO $NBBE $NBCBR 3 $FRAME_STRUCT $SPEED $MAXDELAY $VIDEOBITRATE >$TOTALNAME
		echo FILE $TOTALNAME CREATED!
		let COUNT=COUNT+1
	done
	COUNT=1

	# FLS
	until [ $COUNT -gt $NUMSIM ]; do
		TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_FLS_"$NBUE"U"$CELS"C"".sim"
		../5G-air-simulator SingleCellWithI $CELS $RADIUS $NBUE $NBVOIP $NBVIDEO $NBBE $NBCBR 4 $FRAME_STRUCT $SPEED $MAXDELAY $VIDEOBITRATE >$TOTALNAME
		echo FILE $TOTALNAME CREATED!
		let COUNT=COUNT+1
	done
	COUNT=1

	# EXPRULE
	until [ $COUNT -gt $NUMSIM ]; do
		TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_EXPRULE_"$NBUE"U"$CELS"C"".sim"
		../5G-air-simulator SingleCellWithI $CELS $RADIUS $NBUE $NBVOIP $NBVIDEO $NBBE $NBCBR 5 $FRAME_STRUCT $SPEED $MAXDELAY $VIDEOBITRATE >$TOTALNAME
		echo FILE $TOTALNAME CREATED!
		let COUNT=COUNT+1
	done
	COUNT=1

	# LOGRULE
	until [ $COUNT -gt $NUMSIM ]; do
		TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_LOGRULE_"$NBUE"U"$CELS"C"".sim"
		../5G-air-simulator SingleCellWithI $CELS $RADIUS $NBUE $NBVOIP $NBVIDEO $NBBE $NBCBR 6 $FRAME_STRUCT $SPEED $MAXDELAY $VIDEOBITRATE >$TOTALNAME
		echo FILE $TOTALNAME CREATED!
		let COUNT=COUNT+1
	done
	COUNT=1

	# MT
	until [ $COUNT -gt $NUMSIM ]; do
		TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_MT_"$NBUE"U"$CELS"C"".sim"
		../5G-air-simulator SingleCellWithI $CELS $RADIUS $NBUE $NBVOIP $NBVIDEO $NBBE $NBCBR 7 $FRAME_STRUCT $SPEED $MAXDELAY $VIDEOBITRATE >$TOTALNAME
		echo FILE $TOTALNAME CREATED!
		let COUNT=COUNT+1
	done
	COUNT=1

	# RR
	until [ $COUNT -gt $NUMSIM ]; do
		TOTALNAME=$FILE"_"$COUNT"_"$FILENAME"_RR_"$NBUE"U"$CELS"C"".sim"
		../5G-air-simulator SingleCellWithI $CELS $RADIUS $NBUE $NBVOIP $NBVIDEO $NBBE $NBCBR 8 $FRAME_STRUCT $SPEED $MAXDELAY $VIDEOBITRATE >$TOTALNAME
		echo FILE $TOTALNAME CREATED!
		let COUNT=COUNT+1
	done
	COUNT=1

	# if you add a new schuduler, extend here




	let NBUE=NBUE+INTERVAL 
done
echo SIMULATION FINISHED!
echo CREATING RESULTS REPORT!

# params 1 MINUSERS, 2 MAXUSERS, 3 INTERVAL, 4 FILENAME, 5 FILE, 6 NUMSIM, 7 TypeFlow, Graphic_name

# result shells


./packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VIDEO Packet_Loss_Ratios
./packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VOIP Packet_Loss_Ratios
./packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM INF_BUF Packet-Loss-Ratio
./packet_loss_ratio.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM CBR Packet_Loss_Ratios
./thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VIDEO Throughputs
./thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VOIP Throughputs
./thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM INF_BUF Throughput
./thoughput_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM CBR Throughputs
./delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VIDEO Delays
./delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM VOIP Delays
./delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM INF_BUF Delays
./delay_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM CBR Delays

./spectral_efficiency_comp.sh $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM Spectral-Efficiencies Spec-Eff

#fairness
python3 ./fairness.py $MINUSERS $MAXUSERS $INTERVAL $FILENAME $FILE $NUMSIM
./Graph1.sh "Fairness Index" "Number of users" Fairness-index Fairness-index PF_Y1_Fairness.dat MLWDF_Y1_Fairness.dat EXPPF_Y1_Fairness.dat
rm *.dat
