drop database if exists gerenciamento_projetos;

create database gerenciamento_projetos;
use gerenciamento_projetos;


create table usuarios (
	id int auto_increment primary key,
	nome varchar(127) not null,
	email varchar(127) not null,
	senha varchar(127) not null
);

create table projetos (
	id int auto_increment primary key,
	nome varchar(127) not null,
	descricao text not null,
	estado varchar(127)
);

create table permissoes (
	projeto_id int not null,
	usuario_id int not null,
	admin boolean not null,
	super boolean not null,
	constraint permissoes_projeto_id_fk foreign key (projeto_id) references projetos(id) on delete cascade,
	constraint permissoes_usuario_id_fk foreign key (usuario_id) references usuarios(id) on delete cascade
);

create table subprojetos (
	id int auto_increment primary key,
	description text not null,
	projeto_id int not null,
	estado varchar(127),
	constraint subprojetos_projeto_id_fk foreign key (projeto_id) references projetos(id) on delete cascade
);

create table tarefas (
	id int auto_increment primary key,
	description text not null,
	subprojeto_id int not null,
	estado varchar(127),
	constraint tarefas_subprojeto_id_fk foreign key (subprojeto_id) references subprojetos(id) on delete cascade
);

create table prazos (
	id int auto_increment primary key,
	prazo date not null,
	tarefa_id int not null,
	projeto_id int not null,
	constraint prazos_tarefa_id_fk foreign key (tarefa_id) references tarefas(id) on delete cascade,
	constraint prazos_projeto_id_fk foreign key (projeto_id) references projetos(id) on delete cascade
);

create table foruns (
	id int auto_increment primary key,
	projeto_id int not null,
	nome varchar(127) not null,
	constraint foruns_projeto_id_fk foreign key (projeto_id) references projetos(id) on delete cascade
);

create table mensagens (
	id int auto_increment primary key,
	forum_id int not null,
	mensagem text,
	constraint mensagens_forum_id_fk foreign key (forum_id) references foruns(id) on delete cascade
);

