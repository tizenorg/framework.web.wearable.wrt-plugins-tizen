
typedef void (* async_cb)( int result , void *user_data);


int mock_sync_function(int a, int b);
int mock_delayed_sync_function(int a, int b);
int mock_async_function(int a, int b, async_cb callback, void *user_data);

