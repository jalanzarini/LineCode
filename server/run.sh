g++ -o server server.cpp
./server
lines=`sed -n '$=' server_ami_encoded.txt`
gnuplot -persist <<-EOFMarker
    plot [-2:$lines] [-2:2] "server_ami_encoded.txt" with steps
EOFMarker