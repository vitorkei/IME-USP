# CRUD

[TOC]

## Usuário
- [C] Um **usuário** pode ser registrado autonomamente ao realizar uma ação _CREATE_ - no banco de dados, essa ação é implementada em um `INSERT INTO usuarios (...) VALUES (...)`;
- [R] As informações cadastradas de um **usuário** podem ser acessadas por qualquer um _logado_ no sistema, ao realizar uma ação _READ_ que é implementada no banco de dados com um `SELECT * FROM usuarios WHERE id = ...`;
- [U] A atualização das informações de um **usuário** só podem ser realizadas com um _UPDATE_ feito pelo próprio **usuário** - no banco implementado por `UPDATE usuarios SET ... WHERE id = ...`
- [D] Um registro de **usuário** só pode ser apagado por ele mesmo com uma ação _DELETE_ - `DELETE FROM usuarios WHERE id = ...`


## Permissão
- [C] Uma **permissão** pode ser concedida a um **usuário** e é referente a um único **projeto**. A **permissão** de administrador é concedida a um **usuário** quando este cria um **projeto**. Esta é a ação _CREATE_ implementada no banco de dados por `UPDATE permissoes SET admin = true WHERE projeto_id = ... AND usuario_id = ...`
- [R] Um **usuário** sempre pode ver as suas próprias **permissões** para cada **projeto** que ele participa, e um **usuário** com **permissão** de administrador pode ver todas as **permissões** de todos os **usuários** do **projeto** que ele administra. Esta é a ação _READ_, que é implementada no banco de dados com um `SELECT * FROM permissoes WHERE projeto_id = ...`
- [U] As **permissões** de um **usuário** em um determinado **projeto** podem ser atualizadas por um **usuário** com **permissão** de administrador deste mesmo **projeto**. O administrador pode conferir o status de super usuário ou administrador para um **usuário**. Alternativamente, **usuários** com **permissão** de administrador podem remover **permissões** de outros **usuários**
- [D] A ação _DELETE_ ocorre quando um **projeto** é cancelado e removido do sistema.

## Projeto
- [C] A ação _CREATE_ no **projeto** só pode ser chamada por um **usuário** e é implementada por `INSERT INTO projetos (...) VALUES (...)`
- [R] A ação _READ_ pode ser chamada por todo **usuário** do sistema, utilizando `SELECT * FROM projetos`
- [U] Os **usuários** com **permissão** de administrador do **projeto** são os únicos que podem realizar o _UPDATE_ do **projeto** ao qual ele pertence, chamando `UPDATE projetos SET ... WHERE ...`
- [D] Assim como o _UPDATE_, os **usuários** com **permissão** de administrador são os únicos com poder de chamar _DELETE_, com `DELETE FROM projetos WHERE id = ...`


## Subprojeto
- [C] A ação _CREATE_ em **subprojetos** só pode ser chamada por um administrador ou por um **usuário** previamente autorizado, sempre vinculado a um **projeto**, com `INSERT INTO subprojetos (...) VALUES (...)`
- [R] A exemplo do **projeto**, a ação _READ_ de **subprojeto** é acessível a todos **usuários** do sistema - `SELECT * FROM subprojetos`
- [U] Assim como o _CREATE_, apenas administradores e **usuários** autorizados podem rodar `UPDATE subprojeto SET ... WHERE id = ...`
- [D] Asim como as condições de _CREATE_ e _UPDATE_, o _DELETE_ só pode ser feito por administradores e **usuários** autorizados, com `DELETE FROM subprojetos WHERE id = ...`

## Tarefa
- [C] A ação _CREATE_ em **tarefas** pode ser chamada por qualquer **usuário** vinculado ao **projeto** que contém o **subprojeto** que contém a **tarefa** em questão. A implementação é dada por `INSERT INTO tarefas (...) VALUES (...)`
- [R] A ação _READ_ de uma **tarefa** pode ser chamada por qualquer **usuário** que atende às especificações determinadas na ação _CREATE_ acima, e é implementada por `SELECT * FROM tarefas`
- [U] Assim como o _CREATE_, apenas **usuários** vinculado ao **projeto** que contém o **subprojeto** que contém a **tarefa** em questão podem rodar `UPDATE tarefa SET ... WHERE id = ...`.
- [D] Assim como as ações anteriores, a ação _DELETE_ pode apenas ser realizada por **usuários** com as condições especificadas anteriormente.

## Prazo
- [C] A ação _CREATE_ em **prazos** pode ser chamada por um **usuário** com **permissão** de super ou de administrador.
- [R] A ação _READ_ em **prazos** pode ser chamada por qualquer **usuário** vinculado ao **projeto** que contém a **tarefa** à qual o **prazo** está relacionado.
- [U] Assim como o _CREATE_, a ação _UPDATE_ pode apenas ser chamada por **usuários** com **permissão** de super ou de administrador.
- [D] A ação _DELETE_ pode, também, apenas ser chamada por **usuários** com **permissão** de super ou de administrador.

## Tópico
- [C] Um **tópico** sempre é criado com uma **mensagem**. Portanto, a ação _CREATE_ de **tópico** é chamada automatiamente quando uma nova **mensagem** é criada com a intenção de criar um **tópico** novo. Dessa forma, a ação não pode ser chamada explicitamente por nenhum **usuário** do sistema, mesmo os com **permissão** de super ou de administrador.
- [R] A ação _READ_ pode ser chamada por qualquer **usuário** do **projeto** ao qual o **tópico** está vinculado.
- [U] A ação _UPDATE_ pode ser chamada apenas pelo **usuário** do **projeto** que criou o **tópico**.
- [D] A ação _DELETE_ pode ser chamada apenas pelo **usuário** do **projeto** que criou o **tópico**.

## Mensagem
- [C] A ação _CREATE_ em **mensagem** pode ser chamada por qualquer **usuário** vinculado ao **projeto** ao qual o **tópico** em questão está ligado.
- [R] A ação _READ_ em **mensagem** pode ser chamada por qualquer **usuário** do **projeto** ao qual o **tópico** está vinculado.
- [U] A ação _UPDATE_ em **mensagem** pode ser chamada apenas pelo **usuário** que criou a **mensagem** no **tópico** do **projeto**.
- [D] A ação _DELETE_ em **mensagem** pode ser chamada apenas pelo **usuário** que criou a **mensagem** no **tópico** do **projeto**.

