/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihodge <ihodge@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 18:03:50 by ihodge            #+#    #+#             */
/*   Updated: 2018/04/11 15:47:48 by ihodge           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdio.h>

static int	parameter_type(char *param, int i, int param_num, t_assembler *st)
{
	int			param_type;
	t_label_ref	*label_ref;

	if (!param)
		handle_error("Error: Instruction does not have enough params\n", st);
	param_type = check_param_type(param, i, param_num);
	if (param_type > 0)
	{
		check_lexical_errors(st, param);
		if (param[0] == LABEL_CHAR || param[1] == LABEL_CHAR)
		{
			label_ref = save_label_refs(&st->label_ref);
			label_ref->name = param[0] == DIRECT_CHAR ? ft_strdup(param + 2) :
				ft_strdup(param + 1);
			param[0] == DIRECT_CHAR ? label_ref->dir = 1 : 0;
			label_ref->param_num = param_num;
			label_ref->instruct_num = st->instruct_num;
			label_ref->instruct_offset = st->instruct_offset;
		}
		else
			st->arr[st->instruct_num]->params[param_num] = ft_strdup(param);
		return (param_type);
	}
	handle_error("Error: Wrong parameter type given for instruction\n", st);
	return (0);
}

static void	create_acb(char **instruction, int i, t_assembler *st)
{
	int j;
	int	acb;
	int param_type;

	j = 1;
	acb = 0;
	st->offset++;
	while (j <= g_optab[i].params)
	{
		param_type = parameter_type(instruction[j], i, j - 1, st);
		st->offset += param_type;
		if (instruction[j][0] == DIRECT_CHAR && !g_optab[i].index)
			st->offset += 2;
		if (param_type == 2 && !(instruction[j][0] == DIRECT_CHAR))
			param_type++;
		j == 1 ? acb = param_type << 6 : 0;
		j == 2 ? acb = param_type << 4 | acb : 0;
		j == 3 ? acb = param_type << 2 | acb : 0;
		j++;
	}
	if (j != g_optab[i].params + 1)
		handle_error("Error: Instruction does not have enough params\n", st);
	st->arr[st->instruct_num]->acb = acb;
	if (instruction[j] && instruction[j][0] != COMMENT_CHAR)
		handle_error("Error: Instruction has too many parameters\n", st);
}

static void	convert_instruction(char **instruction, t_assembler *st, int i)
{
	while (g_optab[i].opcode)
	{
		if (ft_strequ(g_optab[i].opstr, instruction[0]))
			break ;
		i++;
	}
	if (!g_optab[i].opcode)
		handle_error("Error: Instruction does not exist\n", st);
	st->arr[st->instruct_num] = ft_memalloc(sizeof(t_instruction));
	st->arr[st->instruct_num]->params = ft_memalloc(sizeof(char*) *
			MAX_ARGS_NUMBER);
	st->arr[st->instruct_num]->op = g_optab[i].opcode;
	st->instruct_offset = st->offset;
	st->offset++;
	if (g_optab[i].acb)
		create_acb(instruction, i, st);
	else
	{
		if (instruction[2] && instruction[2][0] != COMMENT_CHAR)
			handle_error("Error: Instruction has too many parameters\n", st);
		st->offset += parameter_type(instruction[1], i, 0, st);
		if (!g_optab[i].index)
			st->offset += 2;
	}
	st->instruct_num++;
}

void		verify_save_label(t_assembler *st, char *line)
{
	int i;

	i = 0;
	while (line[i] != LABEL_CHAR)
	{
		if (!ft_islower(st->line[i]) && !ft_isdigit(st->line[i])
				&& st->line[i] != '_')
			handle_error("Error: Invalid label name\n", st);
		i++;
	}
	save_labels(&st->label, ft_strsub(line, 0, i), st->offset);
}

void		parse_instructions(t_assembler *st, char *line, int i,
		char **instruction)
{
	while (!ft_iswhitespace(line[i]) && line[i] != COMMENT_CHAR &&
				line[i] != LABEL_CHAR && line[i] != '\0')
	{
		i++;
	}
	if (line[i] == COMMENT_CHAR || line[i] == '\0' || line[i] == '\n')
		return ;
	else if (line[i++] == LABEL_CHAR)
		verify_save_label(st, line);
	else
		i = 0;
	while (ft_iswhitespace(line[i]))
		i++;
	if (line[i] == COMMENT_CHAR || line[i] == '\0' || line[i] == '\n')
		return ;
	else
	{
		instruction = ft_split_by_delims(st->line + i, "\t ,");
		convert_instruction(instruction, st, 0);
	}
	i = 0;
	while (instruction && instruction[i])
		free(instruction[i++]);
	instruction ? free(instruction) : 0;
	st->final_offset = st->offset;
}
