counter=1
cat tophtml
while read line
do
	cleanedLine=$(echo $line |awk '{if(NF==4){printf( "%f %f",$2,$4)}}' )
	if [[ ! -z "$cleanedLine" ]]
	then
		latitude=$(echo $cleanedLine | awk '{print $1}' | sed -n -r 's/^([0-9]{2})(.*)/\1 \2/gp' | awk '{var=($1+($2/60));printf("%f",var)}')
		longitude=$(echo $cleanedLine | awk '{print $2}' | sed -n -r 's/^([0-9]{3})(.*)/\1 \2/gp' | awk '{var=($1+($2/60));printf("%f",var)}')
		# var myLatLng = {lat: 37.3303485, lng: -122.057184167};
		echo 'var myLatLng'$counter' = {lat: '$latitude', lng: -'$longitude'};'
		(( counter++ ))
	fi

done  <"$1"
#awk '{print $1}'|sed -n -r 's/^([0-9]{2})(.*)/\1 \2/gp' | awk '{var=($1+($2/60));printf("%f",var)}'
cat middle
while [[ ! $counter -eq 1 ]]
do
#
#	var marker = new google.maps.Marker({
#    		map: map,
#   		position: myLatLng,
#    		title: 'Hello World!'
#  	});
	((counter--))
	echo "var marker"$counter' = new google.maps.Marker({
               map: map,
               position: myLatLng'$counter',
               title: '\'$counter\''
       });'
done
cat last
