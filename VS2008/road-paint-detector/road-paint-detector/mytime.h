typedef unsigned long long timestamp_t;

 static timestamp_t
 get_timestamp ()
 {
   struct timeval now;
   gettimeofday (&now, NULL);
   return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
 }
