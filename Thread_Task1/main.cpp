#include <QCoreApplication>
#include <cctype>
#include <climits>
#include <iostream>
#include <pthread.h>

using std::cin;
using std::cout;
using std::endl;

/*[brief@]  accumulate numbers from 1 to arg
 *[param@]  arg - how many numbers
 *[return@] sum number from 1 to arg
 */
void * ThreadFunc(void * arg)
{
    int count = *(int*)arg;

    cout << "Count = " << count << endl;

    long int sum = 0;

    for(int i = 1; i <= count; i++)
    {
        sum += i;
    }

    return (void *)sum;
}

/*[brief@]  print error message and finish program
 *[param@]  message to print on display
 *[return@] none
 */
void ErrorExit(const char * csz_message)
{
    if(csz_message != NULL)
    {
        cout << csz_message << endl;
    }

    exit(EXIT_FAILURE);
}

/*[brief@]  create process using fork() or system() or execl()
 *[param@]  argc - args count, argv - arguments
 *[return@] EXIT_SUCCESS if main finish success, else return EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    pthread_t thread_id;
    pthread_attr_t attr;

    // Initialize struct pthread_attr_t
    if (pthread_attr_init(&attr) != 0)
    {
        ErrorExit("prthread_attr_init error!");
    }

    // Initialize stacSize
    // Allocate stack memory
    const int stackSize = PTHREAD_STACK_MIN;
    void * pStackAddr = malloc(stackSize);

    if(pStackAddr == NULL)
    {
        ErrorExit("Can't allocate memory!");
    }

    // Set stack
    int rc = pthread_attr_setstack(&attr, pStackAddr, stackSize);

    if (rc != 0)
    {
        ErrorExit("Set stack error!");
    }

    // Input number
    long int param = 0;

    cout << "Enter count: ";
    cin >> param;

    // Create process
    pthread_create(&thread_id, &attr, &ThreadFunc, (void **)&param); // check params

    // Contain func exec result
    long int * retVal = NULL;
    pthread_join(thread_id, (void**)&retVal);

    // Print result
    cout << "Result " << (long int)retVal << endl;

    // Free struct attr
    rc = pthread_attr_destroy(&attr);

    if (rc != 0)
    {
        ErrorExit("Can't free attr struct!");
    }

    cout << "Execute success!" << endl;

    return a.exec();
}
