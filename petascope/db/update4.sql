-- this table stores latest database update
CREATE TABLE ps_dbupdates (
  id serial NOT NULL,
  update integer,
	primary key (id)
);

insert into ps_dbupdates values (1, 0);
