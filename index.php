
<!DOCTYPE html> 
<html> 
   <head>
      <link href="gdoor.css" rel="stylesheet">
      <title>Garage Door Monitor</title>
   </head>
   <body>
      <?php 

         function PrintState($stateValue){

            switch($stateValue){
               case 1:  
                  return "Opened";
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
      
//         header("refresh: 1"); 
         echo "<h2> The Garage Door </h2>"; 

         class GarageDB extends SQLite3 {
            function __construct() {
               $this->open('/home/bjc/gdoor/exe/garage.db');
            }
         }

         $db = new GarageDB();
         $db->exec('begin');
         $result = $db->query('select * from door_state order by id desc limit 10');

         $first = 1;
         $rec_time = "None";
         $state = "None";
         $boardTemp = "None";

         while ($row = $result->fetchArray()) {

            $rec_time = $row['rec_time'];
            $stateStr = PrintState($row['state']);
            $boardTemp = $row['temperature'];

            if($first == 1){
               echo "<b> {$stateStr} </b> at {$rec_time} <br> <br>";
               $first = 0;

               echo "<table border = 1>
                        <tr>
                           <th>Date and Time</th>
                           <th>State</th>
                           <th>Pi Temperature</th>
                        </tr> ";
            }
            else {

               echo "<tr>
                        <td>$rec_time</td>
                        <td>$stateStr</td>
                        <td>$boardTemp</td>
                    </tr>";
            } 
         }
         echo "</table>"; 

         $db->exec('end');
         $db->close();

      ?> 
   </body> 
</html>
