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

filename=.temp3.data
# echo "set terminal svg size 600,400 dynamic enhanced fname 'arial'  fsize 11 butt solid" >> plot.gnu
# echo "set terminal svg size 600,400" >> plot.gnu
# echo "set output 'bitmap2.svg'" >> plot.gnu
echo "set terminal jpeg size 1000,800 font 'Times_New_Roman,26';" >$filename
echo "set output '$4.jpg';" >>$filename
echo "set style data linespoints" >>$filename
#echo "set style histogram cluster gap 1" >> $filename
# echo "set xtics border in scale 1,0.5 nomirror rotate by -90  offset character 0, 0, 0" >> $filename
# echo "set xtics ($labelxtic)" >> $filename
echo "set grid" >>$filename
#echo "set lmargin 10" >>$filename
echo "set style fill solid 1.0 noborder" >>$filename
#echo "set key off" >>$filename
echo "set key left top" >>$filename
#echo "set datafile missing '-'" >>$filename
echo "set xlabel \"$2\"" >>$filename
echo "set ylabel \"$1\"" >>$filename
#echo "set yrange [0.9:1]" >>$filename
#echo "set xtics  norangelimit ">>$filename
#echo "NO_ANIMATION = 1">>$filename
#echo "set autoscale">>$filename

#echo  "set format y \"%.0t*10^%+03T\"" >> $filename #Pour le PLR

#echo "set title \"$3\"" >>$filename
#echo "set yrange [ *:* ] noreverse nowriteback" >> $filename
#echo "set xrange [ *:* ] noreverse nowriteback" >> $filename
echo "plot '$5' u 2:xtic(1) notitle smooth mcsplines linewidth 3 lc rgb '#ff6d00' ,  \\">>$filename
echo "'' u 2:xtic(1) title 'PF' with points pointsize 4 lc rgb '#ff6d00', \\">>$filename
echo "'$6' using 2:xtic(1) notitle smooth mcsplines linewidth 3 lc rgb '#06d6a0', \\">>$filename
echo "'' u 2:xtic(1) title 'MLWDF' with points pointsize 4 lc rgb '#06d6a0', \\">>$filename
echo "'$7' using 2:xtic(1) notitle smooth mcsplines linewidth 3 lc rgb '#3a86ff', \\" >>$filename
echo "'' u 2:xtic(1) title 'EXP/PF' with points pointsize 4 lc rgb '#3a86ff', \\">>$filename

# if you add a new schuduler, extend here

gnuplot $filename
rm -Rf $filename

