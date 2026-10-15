# TP-ANUAL-ALGORITMOS

Numero de grupo: 10
Integrantes		Nombre								Usuario de GitHub			Parte del TP
				Rodriguez Miranda Lola				lrodriguezmiranda			Normalización
				Mía Taina Enrich					MiaEnrich					Resumen
				Agustina Milagros Riveros Bordes	AgusRiv						Cierre
				Nadia Magali Torres Torres			naitorres					Ventas
				Florencia Lucia Moya				flmoya						Normalización
¿Como se compila?

Normalizacion: 
	cl normalizacion.cpp
	.\normalizacion.exe
	Agarra archivos viejos y los ordena. Al terminar, aparecen mozos.dat
	, varios archivos comandas_dd-mm-aaaa.dat (uno por cada día de ventas) y 
	el inventario.dat queda actualizado con los stocks descontados

Ventas:
	cl ventas.cpp
	.\ventas.exe
	Pregunta la fecha, despues pide número de mozo con su clave, despues producto y cantidad, y asi va cargando las ventas 

Resumen:
	cl resumen.cpp
	.\resumen.exe
	Lee la planilla semanal y la muestra, mozo por mozo, cuántos productos vendio y cuanta comision le corresponde. Al final dice el total de productos del buffet

Cierre:
	cl cierre.cpp
	.\cierre.exe
	Pide datos del año, mes y numero de semana. Al terminar aparece en la planilla semanal con todas las ventas de esa semana juntas y ordenadas por mozo

¿En que orden?
1) Normalizacion para ordenar los archivos 
2) Ventas que necesita que ya exista la lista de mozos y el inventario
3) Cierre, necesita las planillas diarias que se hacen entre ventas y normalizacion 
4) Resumen, usa la planilla semanal que se hace en cierre y dice cuanto vendio cada mozo y cuanta plata lleva de comision

Partes claves de cada programa:
Normalizacion: 
	Extraccion de mozos: recorriendo todas las ventas y mirando si ya tenemos ese nombre en nuestra lista de mozos
	asi no se repiten. Si esta, sumamos la comision. Si no, lo agregamos nuevo con un id (busqueda secuencial)
	Separacion de ventas por dia: recorriendo las ventas, armando el nombre del archivo segun la fecha y grabando todas las ventas de ese dia. 
	Antes de grabar, las ordenamos por idmozo con (ordenamiento por insercion)
	Actualizar el stock del inventario: recorriendo las ventas y, por cada una, buscando el producto en el array por su codigo con busqueda secuencial 
	y restando cantidad al stock. Como la consigna lo pide, se usa "rb+" para leer y escribr sin borrar y no rehacer el inventario ("El inventario.dat no se crea: se va actualizando")

