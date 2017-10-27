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
	estado enum('ABERTO','FECHADO') default 'ABERTO'
);

create table permissoes (
	projeto_id int not null,
	usuario_id int not null,
    permissao enum('ADMIN','SUPER','DEFAULT') default 'DEFAULT',
	constraint permissoes_projeto_id_fk foreign key (projeto_id) references projetos(id) on delete cascade,
	constraint permissoes_usuario_id_fk foreign key (usuario_id) references usuarios(id) on delete cascade
);

create table subprojetos (
	id int auto_increment primary key,
	description text not null,
	projeto_id int not null,
	estado enum('ABERTO','FECHADO') default 'ABERTO',
	constraint subprojetos_projeto_id_fk foreign key (projeto_id) references projetos(id) on delete cascade
);

create table tarefas (
	id int auto_increment primary key,
	description text not null,
	subprojeto_id int not null,
	estado enum('ABERTO','FECHADO') default 'ABERTO',
	constraint tarefas_subprojeto_id_fk foreign key (subprojeto_id) references subprojetos(id) on delete cascade
);

create table prazos (
	id int auto_increment primary key,
	prazo date not null,
	tarefa_id int not null,
	constraint prazos_tarefa_id_fk foreign key (tarefa_id) references tarefas(id) on delete cascade
);

create table topicos (
    id int auto_increment primary key,
    nome varchar(127) not null,
    projeto_id int not null,
    constraint topicos_projeto_id_fk foreign key (projeto_id) references topicos(id) on delete cascade
);

create table mensagens (
	id int auto_increment primary key,
	topico_id int not null,
    usuario_id int not null,
	mensagem text,
    constraint mensagens_topico_id_fk foreign key (topico_id) references topicos(id) on delete cascade,
    constraint mensagens_usuario_id_fk foreign key (usuario_id) references usuarios(id) on delete cascade
);