gnuplot -persist <<-EOFMarker
    plot [-2:100] "client_ami_encoded.txt" with steps
EOFMarker
    