
<!DOCTYPE html> 
<html> 
   <body>
      <?php 

         function PrintState($stateValue){

            switch($stateValue){
               case 1:  
                  return "Open";
                  break;  
               case 2:  
                  return "MovingToClose";
                  break;  
               case 3:  
                  return "Closed";
                  break;  
               case 4:  
                  return "MovingToOpen";
                  break;  
               case 5: 
                  return "NoChange";
                  break;  
               default:  
                  return "None";
                  break;  
            }
         }
      
         header("refresh: 1"); 

         echo "<h2> The Garage Door </h2>"; 

         class GarageDB extends SQLite3 {
            function __construct() {
               $this->open('/home/pi/projects/gdoor/garage.db');
            }
         }

         $db = new GarageDB();
         $db->exec('begin');
         $result = $db->query('select * from door_state order by id desc limit 10');

         $first = 1;
         $rec_time = "None";
         $state = "None";

         while ($row = $result->fetchArray()) {

            $rec_time = $row['rec_time'];
            $stateStr = PrintState($row['state']);

            if($first == 1){
               echo "is <b> {$stateStr} </b> at {$rec_time} <br> <br>";
               $first = 0;

               echo "<table border = 1>
                        <tr>
                           <th>State</th>
                           <th>Date and Time</th>
                        </tr> ";
            }
            else {

               echo "<tr>
                        <td>$stateStr</td>
                        <td>$rec_time</td>
                    </tr>";
            } 
         }
         echo "</table>";


         $db->exec('end');
         $db->close();

      ?> 
   </body> 
</html>
