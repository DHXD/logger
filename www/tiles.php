<?php
    $ttl = 86400; //cache timeout in seconds
 
    $x = intval($_GET['x']);
    $y = intval($_GET['y']);
    $z = intval($_GET['z']);
    $r = strip_tags($_GET['r']);
 
    switch ($r)
    {
      case 'mapnik':
        $r = 'mapnik';
        break;
      case 'mapquest_osm':
        $r = 'mapquest_osm';
        break;
      case 'osma':
      default:
        $r = 'osma';
        break;
    }
 
    $file = "tiles/$r/${z}_${x}_$y.png";
    if (!is_file($file) || filemtime($file)<time()-(86400*30))
    {
      $server = array();
      switch ($r)
      {
      	case 'mapnik':
          $server[] = 'a.tile.openstreetmap.org';
          $server[] = 'b.tile.openstreetmap.org';
          $server[] = 'c.tile.openstreetmap.org';
 
          $url = 'http://'.$server[array_rand($server)];
          $url .= "/".$z."/".$x."/".$y.".png";
          break;
          
        case 'mapquest_osm':
          $server[] = 'otile1.mqcdn.com/tiles/1.0.0/osm';
          $server[] = 'otile2.mqcdn.com/tiles/1.0.0/osm';
          $server[] = 'otile3.mqcdn.com/tiles/1.0.0/osm';
          $server[] = 'otile4.mqcdn.com/tiles/1.0.0/osm';
 
          $url = 'http://'.$server[array_rand($server)];
          $url .= "/".$z."/".$x."/".$y.".png";      // otile1.mqcdn.com/tiles/1.0.0/osm/7_100_56.png
          
          ///$url  = 'http://otile1.mqcdn.com/tiles/1.0.0/osm/7/100/56.png';
          break;
 
      	case 'osma':
      	default:
          $server[] = 'a.tah.openstreetmap.org';
          $server[] = 'b.tah.openstreetmap.org';
          $server[] = 'c.tah.openstreetmap.org';
 
          $url = 'http://'.$server[array_rand($server)].'/Tiles/tile.php';
          $url .= "/".$z."/".$x."/".$y.".png";
          break;
      }
      $ch = curl_init($url);
      $fp = fopen($file, "w");
      curl_setopt($ch, CURLOPT_FILE, $fp);
      curl_setopt($ch, CURLOPT_HEADER, 0);
      curl_exec($ch);
      curl_close($ch);
      fflush($fp);    // need to insert this line for proper output when tile is first requestet
      fclose($fp);
    }
 
    $exp_gmt = gmdate("D, d M Y H:i:s", time() + $ttl * 60) ." GMT";
    $mod_gmt = gmdate("D, d M Y H:i:s", filemtime($file)) ." GMT";
    header("Expires: " . $exp_gmt);
    header("Last-Modified: " . $mod_gmt);
    header("Cache-Control: public, max-age=" . $ttl * 60);
    // for MSIE 5
    header("Cache-Control: pre-check=" . $ttl * 60, FALSE);  
    header ('Content-Type: image/png');
    readfile($file);
  