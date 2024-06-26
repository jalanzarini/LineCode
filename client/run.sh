g++ -o client client.cpp
./client
lines=`sed -n '$=' client_ami_encoded.txt`
gnuplot -persist <<-EOFMarker
    plot [-2:$lines] [-2:2] "client_ami_encoded.txt" with steps
EOFMarker
    