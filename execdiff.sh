#! /bin/tcsh -f

./cl <jp0 >sortida
diff sjp0 sortida
./cl <jp1 >sortida
diff sjp1 sortida
./cl <jp2 >sortida
diff sjp2 sortida
./cl <jp3 >sortida
diff sjp3 sortida
./cl <jp4 >sortida
diff sjp4 sortida
./cl <jp5 >sortida
diff sjp5 sortida
./cl <jp6 >sortida
diff sjp6 sortida
./cl <jp7 >sortida
diff sjp7 sortida
./cl <jp8 >sortida
diff sjp8 sortida
./cl <jp9 >sortida
diff sjp9 sortida
./cl <jp10 >sortida
diff sjp10 sortida
./cl <jp11 >sortida
diff sjp11 sortida
./cl <jp12 >sortida
diff sjp12 sortida
./cl <jp13 >sortida
diff sjp13 sortida

echo "################################################# jp20 #################################################"
./cl <jp20 >sortida execute
diff sjp20 sortida
echo "################################################# jp21 #################################################"
./cl <jp21 >sortida execute
diff sjp21 sortida
echo "################################################# jp22 #################################################"
./cl <jp22 >sortida execute
diff sjp22 sortida
echo "################################################# jp23 #################################################"
./cl <jp23 >sortida execute
diff sjp23 sortida
echo "################################################# jp24 #################################################"
./cl <jp24 >sortida execute
diff sjp24 sortida
echo "################################################# jp25 #################################################"
./cl <jp25 >sortida execute
diff sjp25 sortida
echo "################################################# jp26 #################################################"
./cl <jp26 >sortida execute
diff sjp26 sortida
echo "################################################# jp27 #################################################"
./cl <jp27 >sortida execute
diff sjp27 sortida
echo "################################################# jp28 #################################################"
./cl <jp28 >sortida execute
diff sjp28 sortida
echo "################################################# jp29 #################################################"
./cl <jp29 >sortida execute
diff sjp29 sortida
echo "################################################# jp30 #################################################"
./cl <jp30 >sortida execute
diff sjp30 sortida
echo "################################################# jp31 #################################################"
./cl <jp31 >sortida execute
diff sjp31 sortida
echo "################################################# jp32 #################################################"
./cl <jp32 >sortida execute
diff sjp32 sortida
echo "################################################# jp33 #################################################"
./cl <jp33 >sortida execute
diff sjp33 sortida

rm sortida