use gerenciamento_projetos;

# nome e permissão de todos os usuários que trabalham no projeto "Sistema de Gerenciamento de Requisitos"
select usuarios.nome, permissoes.permissao
from usuarios inner join permissoes on usuarios.id = permissoes.usuario_id 
  inner join projetos on projetos.id = permissoes.projeto_id 
where projetos.nome = 'Sistema de Gerenciamento de Requisitos';

# listar a descrição e prazo de todas as tarefas pendentes do projeto de id = 1 
select  tarefas.description, prazos.prazo
from projetos inner join subprojetos on subprojetos.projeto_id = projetos.id
  inner join tarefas on tarefas.subprojeto_id = subprojetos.id
  inner join prazos on prazos.tarefa_id = tarefas.id
where projetos.id = 1 and tarefas.estado = 'ABERTO';

# contar a quantidade de mensagens no projeto de id = 1
select count(mensagens.id)
from projetos inner join topicos on topicos.projeto_id = projetos.id
  inner join mensagens on mensagens.topico_id = topicos.id
where projetos.id = 1;

# listar ,em ordem crescente, o nome de todos os usuários do sistema que participam de pelo menos 1 projeto.
select usuarios.nome
from usuarios inner join permissoes on usuarios.id = permissoes.usuario_id
group by (usuarios.id) 
having count(usuarios.id) >= 1
order by (usuarios.nome) ASC;
