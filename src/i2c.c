#include "i2c.h"
#include "string.h"

/******************************* I2C Routines *********************************/
/**
  * @brief  Initializes I2C MSP.
  * @param  i2c_handler : I2C handler
  * @retval None
  */
void I2C2_MspInit(I2C_HandleTypeDef *i2c_handler)
{
  GPIO_InitTypeDef  gpio_init_structure;

  /*** Configure the GPIOs ***/
  /* Enable GPIO clock */
  DISCOVERY_I2C2_SCL_SDA_GPIO_CLK_ENABLE();

  /* Configure I2C Tx, Rx as alternate function */
  gpio_init_structure.Pin = DISCOVERY_I2C2_SCL_PIN | DISCOVERY_I2C2_SDA_PIN;
  gpio_init_structure.Mode = GPIO_MODE_AF_OD;
  gpio_init_structure.Pull = GPIO_PULLUP;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = DISCOVERY_I2C2_SCL_SDA_AF;
  HAL_GPIO_Init(DISCOVERY_I2C2_SCL_SDA_GPIO_PORT, &gpio_init_structure);

  HAL_GPIO_Init(DISCOVERY_I2C2_SCL_SDA_GPIO_PORT, &gpio_init_structure);

  /*** Configure the I2C peripheral ***/
  /* Enable I2C clock */
  DISCOVERY_I2C2_CLK_ENABLE();

  /* Force the I2C peripheral clock reset */
  DISCOVERY_I2C2_FORCE_RESET();

  /* Release the I2C peripheral clock reset */
  DISCOVERY_I2C2_RELEASE_RESET();

  /* Enable and set I2Cx Interrupt to a lower priority */
  HAL_NVIC_SetPriority(DISCOVERY_I2C2_EV_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(DISCOVERY_I2C2_EV_IRQn);

  /* Enable and set I2Cx Interrupt to a lower priority */
  HAL_NVIC_SetPriority(DISCOVERY_I2C2_ER_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(DISCOVERY_I2C2_ER_IRQn);
}

/**
  * @brief  DeInitializes I2C MSP.
  * @param  i2c_handler : I2C handler
  * @retval None
  */
void I2C2_MspDeInit(I2C_HandleTypeDef *i2c_handler)
{
  GPIO_InitTypeDef  gpio_init_structure;
  
  /* Configure I2C Tx, Rx as alternate function */
  gpio_init_structure.Pin = DISCOVERY_I2C2_SCL_PIN | DISCOVERY_I2C2_SDA_PIN;
  HAL_GPIO_DeInit(DISCOVERY_I2C2_SCL_SDA_GPIO_PORT, gpio_init_structure.Pin);
  /* Disable GPIO clock */
  DISCOVERY_I2C2_SCL_SDA_GPIO_CLK_DISABLE();
  
  /* Disable I2C clock */
  DISCOVERY_I2C2_CLK_DISABLE();
}

/**
  * @brief  Initializes I2C HAL.
  * @param  i2c_handler : I2C handler
  * @retval None
  */
void I2C2_Init(I2C_HandleTypeDef *i2c_handler)
{
  /* Init the I2C */
  I2C2_MspInit(i2c_handler);
  HAL_I2C_Init(i2c_handler);
  
  /**Configure Analogue filter */
  HAL_I2CEx_ConfigAnalogFilter(i2c_handler, I2C_ANALOGFILTER_ENABLE);  
}

/**
  * @brief  DeInitializes I2C HAL.
  * @param  i2c_handler : I2C handler
  * @retval None
  */
void I2C2_DeInit(I2C_HandleTypeDef *i2c_handler)
{  /* DeInit the I2C */
  I2C2_MspDeInit(i2c_handler);
  HAL_I2C_DeInit(i2c_handler); 
}

/******************************* I2C Routines *********************************/
/**
  * @brief  Initializes I2C MSP.
  * @param  i2c_handler : I2C handler
  * @retval None
  */
void I2C1_MspInit(I2C_HandleTypeDef *i2c_handler)
{
  GPIO_InitTypeDef  gpio_init_structure;

  /*** Configure the GPIOs ***/
  /* Enable GPIO clock */
  DISCOVERY_I2C1_SCL_SDA_GPIO_CLK_ENABLE();

  /* Configure I2C Tx, Rx as alternate function */
  gpio_init_structure.Pin = DISCOVERY_I2C1_SCL_PIN | DISCOVERY_I2C1_SDA_PIN;
  gpio_init_structure.Mode = GPIO_MODE_AF_OD;
  gpio_init_structure.Pull = GPIO_PULLUP;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = DISCOVERY_I2C1_SCL_SDA_AF;
  HAL_GPIO_Init(DISCOVERY_I2C1_SCL_SDA_GPIO_PORT, &gpio_init_structure);

  HAL_GPIO_Init(DISCOVERY_I2C1_SCL_SDA_GPIO_PORT, &gpio_init_structure);

  /*** Configure the I2C peripheral ***/
  /* Enable I2C clock */
  DISCOVERY_I2C1_CLK_ENABLE();

  /* Force the I2C peripheral clock reset */
  DISCOVERY_I2C1_FORCE_RESET();

  /* Release the I2C peripheral clock reset */
  DISCOVERY_I2C1_RELEASE_RESET();

  /* Enable and set I2Cx Interrupt to a lower priority */
  HAL_NVIC_SetPriority(DISCOVERY_I2C1_EV_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(DISCOVERY_I2C1_EV_IRQn);

  /* Enable and set I2Cx Interrupt to a lower priority */
  HAL_NVIC_SetPriority(DISCOVERY_I2C1_ER_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(DISCOVERY_I2C1_ER_IRQn);
}

/**
  * @brief  DeInitializes I2C MSP.
  * @param  i2c_handler : I2C handler
  * @retval None
  */
void I2C1_MspDeInit(I2C_HandleTypeDef *i2c_handler)
{
  GPIO_InitTypeDef  gpio_init_structure;
  
  /* Configure I2C Tx, Rx as alternate function */
  gpio_init_structure.Pin = DISCOVERY_I2C1_SCL_PIN | DISCOVERY_I2C1_SDA_PIN;
  HAL_GPIO_DeInit(DISCOVERY_I2C1_SCL_SDA_GPIO_PORT, gpio_init_structure.Pin);
  /* Disable GPIO clock */
  DISCOVERY_I2C1_SCL_SDA_GPIO_CLK_DISABLE();
  
  /* Disable I2C clock */
  DISCOVERY_I2C1_CLK_DISABLE();
}

/**
  * @brief  Initializes I2C HAL.
  * @param  i2c_handler : I2C handler
  * @retval None
  */
void I2C1_Init(I2C_HandleTypeDef *i2c_handler)
{
  /* Init the I2C */
  I2C1_MspInit(i2c_handler);
  HAL_I2C_Init(i2c_handler);
  
  /**Configure Analogue filter */
  HAL_I2CEx_ConfigAnalogFilter(i2c_handler, I2C_ANALOGFILTER_ENABLE);  
}

/**
  * @brief  DeInitializes I2C HAL.
  * @param  i2c_handler : I2C handler
  * @retval None
  */
void I2C1_DeInit(I2C_HandleTypeDef *i2c_handler)
{  /* DeInit the I2C */
  I2C1_MspDeInit(i2c_handler);
  HAL_I2C_DeInit(i2c_handler); 
}

void I2Cx_Init(I2C_HandleTypeDef *i2c_handler){
    switch ((uint32_t)i2c_handler->Instance)
    {
        case I2C1_BASE:
            I2C1_Init(i2c_handler);
        break;
        case I2C2_BASE:
            I2C2_Init(i2c_handler);
        break;
    }
}

void I2Cx_DeInit(I2C_HandleTypeDef *i2c_handler){
    switch ((uint32_t)i2c_handler->Instance)
    {
        case I2C1_BASE:
            I2C1_DeInit(i2c_handler);
        break;
        case I2C2_BASE:
            I2C2_DeInit(i2c_handler);
        break;
    }
}

void IC2x_Init_Handler(I2C_HandleTypeDef *i2c_handler){
    memset(i2c_handler, 0, sizeof(*i2c_handler));
    i2c_handler->Init.Timing           = DISCOVERY_I2C_TIMING;
    i2c_handler->Init.OwnAddress1      = 0;
    i2c_handler->Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    i2c_handler->Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    i2c_handler->Init.OwnAddress2      = 0;
    i2c_handler->Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    i2c_handler->Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE; 
}

/**
  * @brief  Manages error callback by re-initializing I2C.
  * @param  i2c_handler : I2C handler
  * @param  Addr: I2C Address
  * @retval None
  */
void I2Cx_Error(I2C_HandleTypeDef *i2c_handler, uint8_t Addr)
{
  /* De-initialize the I2C communication bus */
  HAL_I2C_DeInit(i2c_handler);
  
  /* Re-Initialize the I2C communication bus */
  I2Cx_Init(i2c_handler);
}


/**
  * @brief  Reads multiple data.
  * @param  i2c_handler : I2C handler
  * @param  Addr: I2C address
  * @param  Reg: Reg address
  * @param  MemAddress: memory address
  * @param  Buffer: Pointer to data buffer
  * @param  Length: Length of the data
  * @retval HAL status
  */
HAL_StatusTypeDef I2Cx_ReadMultiple(I2C_HandleTypeDef *i2c_handler, uint8_t Addr, uint16_t Reg, uint16_t MemAddress, uint8_t *Buffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_I2C_Mem_Read(i2c_handler, Addr, (uint16_t)Reg, MemAddress, Buffer, Length, 1000);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* I2C error occured */
    I2Cx_Error(i2c_handler, Addr);
  }
  return status;
}

HAL_StatusTypeDef I2Cx_Read(I2C_HandleTypeDef *i2c_handler, uint8_t Addr, uint16_t Reg, uint16_t MemAddSize, uint8_t value){
  return I2Cx_ReadMultiple(i2c_handler,Addr, Reg, MemAddSize, &value, 1);
}


/**
  * @brief  Writes a value in a register of the device through BUS in using DMA mode.
  * @param  i2c_handler : I2C handler
  * @param  Addr: Device address on BUS Bus.
  * @param  Reg: The target register address to write
  * @param  MemAddress: memory address
  * @param  Buffer: The target register value to be written
  * @param  Length: buffer size to be written
  * @retval HAL status
  */
HAL_StatusTypeDef I2Cx_WriteMultiple(I2C_HandleTypeDef *i2c_handler, uint8_t Addr, uint16_t Reg, uint16_t MemAddress, uint8_t *Buffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_I2C_Mem_Write(i2c_handler, Addr, (uint16_t)Reg, MemAddress, Buffer, Length, 1000);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initiaize the I2C Bus */
    I2Cx_Error(i2c_handler, Addr);
  }
  return status;
}

HAL_StatusTypeDef I2Cx_Write(I2C_HandleTypeDef *i2c_handler, uint8_t Addr, uint16_t Reg, uint16_t MemAddress, uint8_t value){
  return I2Cx_WriteMultiple(i2c_handler, Addr, Reg, MemAddress, &value, 1);
}


/**
  * @brief  Checks if target device is ready for communication. 
  * @note   This function is used with Memory devices
  * @param  i2c_handler : I2C handler
  * @param  DevAddress: Target device address
  * @param  Trials: Number of trials
  * @retval HAL status
  */
HAL_StatusTypeDef I2Cx_IsDeviceReady(I2C_HandleTypeDef *i2c_handler, uint16_t DevAddress, uint32_t Trials)
{ 
  return (HAL_I2C_IsDeviceReady(i2c_handler, DevAddress, Trials, 1000));
}