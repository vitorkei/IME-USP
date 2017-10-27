use gerenciamento_projetos;

INSERT INTO usuarios (nome, email, senha) VALUES
("JEF","jef@ime.usp.br","intereintra"),
("Lucas Stefan","lucastefan9@gmail.com","senhadolucas"),
("Fábio Tanaka","fhktanaka@gmail.com","senhadofabio"),
("Luiz Gyrotto","luiz.girotto@usp.br","senhadoluiz"),
("Virtor Kei","vitor_kei9@hotmail.com","senhadokei"),
("Jão Daniel","jotaf.daniel@gmail.com","senhadojoao");

INSERT INTO projetos (nome,descricao) VALUES
("Sistema de Gerenciamento de Requisitos","Sistema de gerenciamento de requisitos, projeto da matéria MAC0350 do BCC-IME-USP");

INSERT INTO permissoes (projeto_id,usuario_id,permissao) VALUES
(1,1,'ADMIN');

INSERT INTO topicos (projeto_id,nome) VALUES
(1,"Forum geral");

INSERT INTO permissoes (projeto_id,usuario_id) VALUES
(1,2),(1,3),(1,4),(1,5),(1,6);

INSERT INTO subprojetos (description,projeto_id) VALUES
("EP2",1);

INSERT INTO tarefas (description,subprojeto_id) VALUES
("revisão do EP1",1),
("criação do modelo físico (SQL)",1),
("população do banco de dados (script)",1),
("consulta (query)",1);

INSERT INTO prazos (prazo,tarefa_id) VALUES
('2017-10-26',1),('2017-10-26',2),('2017-10-26',3),('2017-10-26',4);

