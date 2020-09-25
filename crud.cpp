#include <bits/stdc++.h>
#include <pqxx/pqxx>
#include <string>
#include <iostream>
using namespace std;
using namespace pqxx;
void create(connection &C)
{
    string sql = "INSERT INTO EMPLOYEE(ID,NAME,AGE,SALARY,ADDRESS) VALUES (";
    cout << "Enter employee id: ";
    string temp; 
    cin >> temp;
    sql = sql + temp + ",";
    cout << "Enter name of the employee: ";
    cin.ignore();
    getline(cin, temp);
    sql = sql + "'" + temp + "',";
    cout << "Enter age of the employee: "; 
    cin >> temp; 
    sql = sql + temp + ",";
    cout << "Enter the salary of the employee: ";
    cin.ignore();
    cin >> temp;
    sql = sql + temp + ",";
    cout << "Enter address of the employee: ";
    cin.ignore();
    getline(cin, temp);
    sql = sql + "'" + temp + "');"; 
    work W(C);
    W.exec(sql);
    cout << "Press y/Y key to save and any other key to undo: ";
    cin >> temp;
    if (temp == "y" || temp == "Y")
        W.commit();
}
void display(string id,connection &C)
{

    string sqltemp="SELECT * FROM EMPLOYEE WHERE ID="+id+";" ;
    nontransaction N(C);
    result R(N.exec(sqltemp));
    cout<<"Current entry: \n";
    for(result::iterator c=R.begin();c!=R.end();++c)
    cout << "ID: " << c[0] << " Name: " << c[1] << " Age: " << c[2] << " Salary: " << c[3] << " Address: " << c[4] << endl;

}
void update(connection &C)
{
    string id;
    cout<<"Enter id of the employee: ";
    cin>>id;
    display(id,C);
    string sql="UPDATE EMPLOYEE SET ";
    cout<<"What do you want to update?\n 1. Name 2. Age 3.Salary 4. Address\n";
    string b[5]={"","NAME","AGE","SALARY","ADDRESS"};

    int cnt=0;
    string choice="y";
    while(choice=="y"||choice=="Y")
    {
        int n;
        cout<<"Enter your choice number: ";
        cin>>n; 
        string temp;
        cout<<"Enter the new value for your choice: ";
        cin.ignore();
        getline(cin,temp);
        if(n==1||n==4)
        {
            if(cnt==0)
            sql=sql+b[n]+" = " +"'"+temp+"'";
            else 
            sql=sql+","+b[n]+" = "+"'"+temp+"'";
        }
        else 
        {
            if(cnt==0)
            sql=sql+b[n]+" = "+temp;
            else 
            sql=sql+","+b[n]+" = "+temp;
        }
        ++cnt;
        cout<<"Do you want update some more columns?\nPress y/Y to continue: ";
        cin>>choice;
    }
    sql=sql+" WHERE ID="+id+";";
    cout<<sql<<endl;
    work W(C);
    W.exec(sql);
    cout<<"Do you want to save the changes?\nEnter y/Y to continue: ";
    string yes;
    cin>>yes;
    if(yes=="y"||yes=="Y")
    {
        W.commit();
        sql="SELECT * FROM EMPLOYEE WHERE ID = "+ id+";";
        nontransaction n(C);
        result r(n.exec(sql));
        cout<<"Current entry after the update: \n";
        for(result::iterator c=r.begin();c!=r.end();++c)
        cout << "ID: " << c[0] << " Name: " << c[1] << " Age: " << c[2] << " Salary: " << c[3] << " Address: " << c[4] << endl;

    }


}
void deleteEntry(connection &C)
{
    string id;
    cout<<"Enter the id of the employee: ";
    cin>>id;
    cout<<"Are you sure, you want to delete the record with id "<<id<<" ?\nPress y/Y to continue: ";
    string yes;
    cin>>yes;
    if(yes=="y"||yes=="Y")
    {
        string sql="DELETE FROM EMPLOYEE WHERE ID = "+id+";";
        work W(C);
        W.exec(sql);
        W.commit();
    }
}
void read(connection &C)
{
    string sql = "SELECT * FROM EMPLOYEE";
    nontransaction N(C);
    result R(N.exec(sql));
    for (result::iterator c = R.begin(); c != R.end(); ++c)
    {
        cout << "ID: " << c[0] << " Name: " << c[1] << " Age: " << c[2] << " Salary: " << c[3] << " Address: " << c[4] << endl;
    }
}
int main(int argc, char *argv[])
{ 
    string sql;
    try
    {
        connection C("dbname = postgres user = postgres password = wishtodie@123 hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open())
        {
            cout << "Opened database successfully: " << C.dbname() << endl;
        }
        else
        {
            cout << "Cannot open the database" << endl;
            return 1;
        }
        // Comment the next four lines once the database is created.....
        sql="CREATE TABLE EMPLOYEE(ID INT PRIMARY KEY,NAME TEXT,AGE INT,SALARY INT, ADDRESS CHAR(50));";
        work W(C);
        W.exec(sql);
        W.commit();
        string str = "y";
        while (str == "y" || str == "Y")
        { 
            cout << "Pick your choice" << endl;
            cout << "1. Create\n2. Read\n3. Update\n4. Delete\n";
            int choice;
            cin >> choice;
            if (choice == 1)
                create(C);
            else if (choice == 2)
                read(C);
            else if (choice == 3)
                update(C);
            else if (choice == 4)
                deleteEntry(C);
            else
                cout << "Invalid input.....try again....\n";
            cout << "Press y/Y to continue and any other key to stop: -";
            cin >> str;
        }
    }
    catch (const exception &e)
    {
        cerr << e.what() << endl;
        return 1;
    }
}