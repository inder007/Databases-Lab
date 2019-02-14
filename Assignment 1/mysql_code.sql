sudo apt update
sudo apt-install mysql-server
sudo mysql_secure_installation

create database one
show databases
use one

create table department( 
name varchar(50) not null, 
id varchar(20) not null, 
primary key(id));

create table courses( 
name varchar(50) not null, 
id varchar(20) not null, 
dept varchar(20) not null,
foreign key(dept) reference department(id),
primary key(id));

create table students( 
name varchar(50) not null, 
id varchar(20) not null, 
dept varchar(20) not null,
foreign key(dept) reference department(id),
primary key(id));

create table lecturer( 
name varchar(50) not null, 
id varchar(20) not null, 
dept varchar(20) not null,
foreign key(dept) reference department(id),
primary key(id));

create table modules( 
name varchar(50) not null, 
id varchar(20) not null, 
lecturer_id varchar(20) not null,
foreign key(lecturer_id) reference lecturer(id),
primary key(id));

create table tutors( 
l_id varchar(20) not null, 
s_id varchar(20) not null, 
foreign key(l_id) references lecturer(id), 
foreign key(s_id) references students(id));

create table cm_table( 
c_id varchar(20) not null, 
m_id varchar(20) not null, 
foreign key(c_id) references courses(id),
 foreign key(m_id) references modules(id));

create table sm_table( 
c_id varchar(20) not null, 
m_id varchar(20) not null, 
foreign key(c_id) references courses(id),
 foreign key(m_id) references modules(id));

insert into department values
('computer science', 'CSE'),
('electrcial and comm.', 'ece'),
('biotech', 'bt');

select * from department 
where id='CSE';

delete from department
where id='bt';

drop table department;
OR
delete from department;